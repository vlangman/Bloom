using UnityEngine;
using System.Collections.Generic;
using UnityEngine.Experimental.Rendering;
using ComputeShaderUtility;
public class ComputeOrganisms : MonoBehaviour
{

	public Material organismMaterial;
	public Material pheromoneMaterial;
	public ComputeShader computeSimulation;
	public Transform PheromoneLayer;

	// //PRIVATE

	private Dictionary<int, ComputeShader> _kernels = new Dictionary<int, ComputeShader>();

	private int[] args;

	//ORGANISM
	private ComputeBuffer argsBuffer;
	private List<Organism> organismList = new List<Organism>();
	private ComputeBuffer organismBuffer;
	private ComputeBuffer organismFilteredResultBuffer;

	//NEURON
	private List<Neuron> neuronList = new List<Neuron>();
	private ComputeBuffer neuronBuffer;
	private ComputeBuffer neuronLookupBuffer;

	//PHEROMONE
	private List<Neuron> pheromoneList = new List<Neuron>();
	private ComputeBuffer pheromoneBuffer;

	private Vector2 resolution = new Vector2(32, 32);
	private uint organismCount = 0;
	private uint neuronCount = 0;

	private Bounds bounds;

	void Start()
	{

		//just to make sure the buffer are clean
		release();

		organismCount = (uint)(resolution.x * resolution.y);
		neuronCount = organismCount * Globals.NeuronCount;

		GenerateTestData();

		InitializeBuffers();

		//Args for indirect draw
		args = new int[]
		{
			(int)1, //vertex count per instance
			(int)organismCount, //instance count
			(int)0, //start vertex location
			(int)0 //start instance location
		};
		argsBuffer = new ComputeBuffer(args.Length, Globals.UINT_SIZE, ComputeBufferType.IndirectArguments);
		argsBuffer.SetData(args);

		//just a big enough bounds for drawing
		bounds = new Bounds(Vector3.zero, new Vector3(resolution.x, resolution.y, 1));



		transform.GetComponentInChildren<MeshRenderer>().material.mainTexture = organismTexture;
		PheromoneLayer.GetComponentInChildren<MeshRenderer>().material.mainTexture = pheromoneTexture;

	}
	public FilterMode filterMode = FilterMode.Point;
	public GraphicsFormat format = ComputeHelper.defaultGraphicsFormat;
	[SerializeField, HideInInspector] protected RenderTexture organismTexture;
	[SerializeField, HideInInspector] protected RenderTexture pheromoneTexture;

	void BindTextures()
	{
		// Create render textures
		ComputeHelper.CreateRenderTexture(ref organismTexture, (int)resolution.x, (int)resolution.y, filterMode, format);
		ComputeHelper.CreateRenderTexture(ref pheromoneTexture, (int)resolution.x, (int)resolution.y, filterMode, format);

		organismTexture.enableRandomWrite = true;
		pheromoneTexture.enableRandomWrite = true;

		// Assign textures
		computeSimulation.SetTexture(computeSimulation.FindKernel("ProcessNeurons"), "organismTexture", organismTexture);
		computeSimulation.SetTexture(computeSimulation.FindKernel("ProcessNeurons"), "pheromoneTexture", pheromoneTexture);

		computeSimulation.SetTexture(computeSimulation.FindKernel("ProcessPheromones"), "pheromoneTexture", pheromoneTexture);

	}

	void Update()
	{
		ClearOutRenderTexture(ref organismTexture);
		//Reset count
		organismFilteredResultBuffer.SetCounterValue(0);

		foreach (var kernelDirect in _kernels.Keys)
		{
			//Direct dispatch to do filter
			_kernels[kernelDirect].SetFloat("_deltaTime", Time.deltaTime);
			_kernels[kernelDirect].SetFloat("_time", Time.time);
			_kernels[kernelDirect].SetVector("_resolution", resolution);
			_kernels[kernelDirect].SetInt("_seed", Random.Range(int.MinValue, int.MaxValue));
			_kernels[kernelDirect].Dispatch(kernelDirect, (int)resolution.x, (int)resolution.y, 1);
		}


		//Copy Count - visually no change but this is necessary in terms of performance!
		//because without this, shader will draw full amount of organisms, just overlapping
		//Check Profiler > GPU > Hierarchy search Graphics.DrawProcedural > GPU time
		//4 is the offset byte. "organismCount" is the second int in args[], and 1 uint = 4 bytes
		// ComputeBuffer.CopyCount(organismFilteredResultBuffer, argsBuffer, Globals.UINT_SIZE);

		//Draw
		//3*4 is the offset byte, where the indirect draw in args starts
		// Graphics.DrawProceduralIndirect(organismMaterial, bounds, MeshTopology.Points, argsBuffer, 0);
	}

	void InitializeBuffers()
	{
		//setup all the kernels for lookup later
		_kernels.Add(computeSimulation.FindKernel("ProcessNeurons"), computeSimulation);
		_kernels.Add(computeSimulation.FindKernel("ProcessPheromones"), computeSimulation);

		Debug.Log($"ORGANISM SIZE: ${Globals.OrganismSize} BUFFER SIZE : {Globals.OrganismSize * organismCount / 1e6} MB");
		Debug.Log($"NEURON SIZE: ${Globals.NeuronSize} BUFFER SIZE : {Globals.NeuronSize * organismCount / 1e6} MB");
		Debug.Log($"PHEROMONE SIZE: ${Globals.PheromoneSize} BUFFER SIZE : {Globals.PheromoneSize * organismCount / 1e6} MB");

		//organismBuffer,
		organismBuffer = new ComputeBuffer((int)organismCount, Globals.OrganismSize);
		organismBuffer.SetData(organismList);

		//organisms neuron buffer,
		neuronBuffer = new ComputeBuffer((int)neuronCount, Globals.NeuronSize);
		neuronBuffer.SetData(neuronList);

		//pheromone buffer ,
		pheromoneBuffer = new ComputeBuffer((int)organismCount, Globals.PheromoneSize);
		List<Pheromone> data = new List<Pheromone>((int)organismCount);
		// for (int i = 0; i < organismCount; i++)
		// {
		// 	data.Add(new Pheromone
		// 	{
		// 		colour = new Vector4(1, 0, 0, 0.6f)
		// 	});
		// }

		pheromoneBuffer.SetData(data);

		//filtered result buffer, storing only the idx value of a organism
		organismFilteredResultBuffer = new ComputeBuffer((int)organismCount, Globals.UINT_SIZE, ComputeBufferType.Append);
		organismMaterial.SetBuffer("organismBuffer", organismBuffer);
		organismMaterial.SetBuffer("organismResult", organismFilteredResultBuffer);
		organismMaterial.SetBuffer("pheromoneBuffer", pheromoneBuffer);

		this.BindTextures();


		//set the buffers for all compute shaders
		foreach (var kernelDirect in _kernels.Keys)
		{
			_kernels[kernelDirect].SetBuffer(kernelDirect, "organismFiltered", organismFilteredResultBuffer);
			_kernels[kernelDirect].SetBuffer(kernelDirect, "organismBuffer", organismBuffer);
			_kernels[kernelDirect].SetBuffer(kernelDirect, "neuronBuffer", neuronBuffer);
			_kernels[kernelDirect].SetBuffer(kernelDirect, "pheromoneBuffer", pheromoneBuffer);
		}
	}

	void GenerateTestData()
	{
		// Init prefab neurons for generation and storage. will be copied into base Neuron later for GPU write
		var prefabNeurons = new List<INeuron>();
		for (uint x = 0; x < resolution.x; x++)
		{
			for (uint y = 0; y < resolution.y; ++y)
			{


				// Indexes
				// (row * length_of_row) + column; 
				uint index = y * (uint)resolution.x + x;

				var Organism = OrganismFactory.CreateSlimeBase(index, ref prefabNeurons);


				Organism.position = new Vector2(x, y);
				Organism.colour = new Vector4(0, 0, 1, 1);

				Organism.alive = 0;
				if (x > 10 && x < 20 && y > 10 && y < 20)
				{
					Organism.alive = 1;
					Organism.colour = new Vector4(1, 1, 1, 1);
				}


				organismList.Add(Organism);

			}
		}

		prefabNeurons.ForEach((INeuron neuron) =>
		{
			neuronList.Add(neuron.InitializeBase());
		});

	}

	public void ClearOutRenderTexture(ref RenderTexture renderTexture)
	{
		RenderTexture rt = RenderTexture.active;
		RenderTexture.active = renderTexture;
		GL.Clear(true, true, Color.clear);
		RenderTexture.active = rt;
	}



	private void release()
	{
		if (organismFilteredResultBuffer != null)
		{
			organismFilteredResultBuffer.Dispose();
			organismFilteredResultBuffer.Release();
			organismFilteredResultBuffer = null;

		}
		if (organismBuffer != null)
		{
			organismBuffer.Dispose();
			organismBuffer.Release();
			organismBuffer = null;
		}
		if (neuronBuffer != null)
		{
			neuronBuffer.Dispose();
			neuronBuffer.Release();
			neuronBuffer = null;
		}
		if (argsBuffer != null)
		{
			argsBuffer.Dispose();
			argsBuffer.Release();
			argsBuffer = null;
		}
		if (pheromoneBuffer != null)
		{
			pheromoneBuffer.Dispose();
			pheromoneBuffer.Release();
			pheromoneBuffer = null;
		}
	}
	void OnDestroy()
	{
		release();
	}
	void OnApplicationQuit()
	{
		release();
	}
}