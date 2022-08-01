using UnityEngine;
using System.Collections.Generic;

public class ComputeOrganisms : MonoBehaviour
{

	public Material material;
	public ComputeShader computeNeurons;

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


	private Vector2 resolution = new Vector2(512, 512);
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
		argsBuffer = new ComputeBuffer(args.Length, sizeof(int), ComputeBufferType.IndirectArguments);
		argsBuffer.SetData(args);

		//just a big enough bounds for drawing
		bounds = new Bounds(Vector3.zero, new Vector3(resolution.x, resolution.y, 1));
	}

	void Update()
	{
		//Reset count
		organismFilteredResultBuffer.SetCounterValue(0);

		foreach (var kernelDirect in _kernels.Keys)
		{
			//Direct dispatch to do filter
			_kernels[kernelDirect].SetFloat("_deltaTime", Time.deltaTime);
			_kernels[kernelDirect].SetFloat("_time", Time.time);
			_kernels[kernelDirect].SetVector("_resolution", resolution);
			_kernels[kernelDirect].Dispatch(kernelDirect, (int)resolution.x / 16, (int)resolution.y / 16, 1);
		}



		//Copy Count - visually no change but this is necessary in terms of performance!
		//because without this, shader will draw full amount of organisms, just overlapping
		//Check Profiler > GPU > Hierarchy search Graphics.DrawProcedural > GPU time
		//4 is the offset byte. "organismCount" is the second int in args[], and 1 int = 4 bytes
		ComputeBuffer.CopyCount(organismFilteredResultBuffer, argsBuffer, 4);

		//Draw
		//3*4 is the offset byte, where the indirect draw in args starts
		Graphics.DrawProceduralIndirect(material, bounds, MeshTopology.Points, argsBuffer, 0);
	}

	void InitializeBuffers()
	{
		//setup all the kernels for lookup later
		_kernels.Add(computeNeurons.FindKernel("ProcessNeurons"), computeNeurons);

		//organismBuffer,
		Debug.Log($"ORGANISM SIZE : {Globals.OrganismSize}");
		organismBuffer = new ComputeBuffer((int)organismCount, Globals.OrganismSize);
		organismBuffer.SetData(organismList);

		//organisms neuron buffers,
		Debug.Log($"NEURON SIZE : {Globals.NeuronSize}");
		neuronBuffer = new ComputeBuffer((int)neuronCount, Globals.NeuronSize);
		neuronBuffer.SetData(neuronList);

		//filtered result buffer, storing only the idx value of a organism
		organismFilteredResultBuffer = new ComputeBuffer((int)organismCount, sizeof(uint), ComputeBufferType.Append);
		material.SetBuffer("organismBuffer", organismBuffer);
		material.SetBuffer("organismResult", organismFilteredResultBuffer);


		//set the buffers for all compute shaders
		foreach (var kernelDirect in _kernels.Keys)
		{
			_kernels[kernelDirect].SetBuffer(kernelDirect, "organismFiltered", organismFilteredResultBuffer);
			_kernels[kernelDirect].SetBuffer(kernelDirect, "organismBuffer", organismBuffer);
			_kernels[kernelDirect].SetBuffer(kernelDirect, "neuronBuffer", neuronBuffer);
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


				// (row * length_of_row) + column; // Indexes
				uint index = y * (uint)resolution.x + x;

				var Organism = OrganismFactory.CreateSlimeBase(index, ref prefabNeurons);

				if (x >= 200 && x <= 400 && y >= 200 && y <= 400)
					Organism.alive = 1;

				Organism.position = new Vector2(x, y);
				Organism.color = Color.yellow;
				organismList.Add(Organism);

			}
		}

		prefabNeurons.ForEach((INeuron neuron) =>
		{
			neuronList.Add(neuron.InitializeBase());
		});

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