using UnityEngine;
using System.Collections;

public class ComputeOrganisms : MonoBehaviour
{
	//The stride passed when constructing the buffer must match structure size, be a multiple of 4 and less than 2048
	struct Organism
	{

		public uint idx;//4 bytes 

		public uint alive;//4  bytes

		public Vector2 orientation;//8  bytes

		public Vector2 position;//8  bytes

		public Color color;// 16 bytes: (4*4)

		public int GetSize()
		{
			return
				//idx
				4 +
				// alive
				4 +
				// orientation
				8 +
				//position
				8 +
				//color
				16;
		}
	};

	public Material material;
	public ComputeShader computeShader;

	private int _kernelDirect;
	private ComputeBuffer organismBuffer;
	private ComputeBuffer organismFilteredResultBuffer;
	private ComputeBuffer argsBuffer;
	private int[] args;
	private Organism[] organismList;


	private Vector2 resolution = new Vector2(512, 512);
	private uint organismCount = 0;

	private Bounds bounds;

	void Start()
	{

		organismCount = (uint)(resolution.x * resolution.y);
		//just to make sure the buffer are clean
		release();

		//kernels
		_kernelDirect = computeShader.FindKernel("UpdateOrganism");

		// Init organisms position
		organismList = new Organism[organismCount];
		for (uint x = 0; x < resolution.x; x++)
		{
			for (uint y = 0; y < resolution.y; ++y)
			{
				// (row * length_of_row) + column; // Indexes
				uint index = y * (uint)resolution.x + x;

				if (x >= 200 && x <= 400 && y >= 200 && y <= 400)
					organismList[index].alive = 1;

				organismList[index].idx = index;
				organismList[index].position = new Vector2(x, y);
				organismList[index].color = Color.yellow;
			}
		}


		//organismBuffer, for rendering
		organismBuffer = new ComputeBuffer((int)organismCount, organismList[0].GetSize());
		organismBuffer.SetData(organismList);

		//filtered result buffer, storing only the idx value of a organism
		organismFilteredResultBuffer = new ComputeBuffer((int)organismCount, sizeof(uint), ComputeBufferType.Append);

		//bind buffer to computeShader and Material
		computeShader.SetBuffer(_kernelDirect, "organismFiltered", organismFilteredResultBuffer);
		computeShader.SetBuffer(_kernelDirect, "organismBuffer", organismBuffer);
		material.SetBuffer("organismBuffer", organismBuffer);
		material.SetBuffer("organismResult", organismFilteredResultBuffer);

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

		//Direct dispatch to do filter
		computeShader.SetFloat("_deltaTime", Time.deltaTime);
		computeShader.SetFloat("_time", Time.time);
		computeShader.SetVector("_resolution", resolution);
		computeShader.Dispatch(_kernelDirect, (int)resolution.x / 16, (int)resolution.y / 16, 1);

		//Copy Count - visually no change but this is necessary in terms of performance!
		//because without this, shader will draw full amount of organisms, just overlapping
		//Check Profiler > GPU > Hierarchy search Graphics.DrawProcedural > GPU time
		//4 is the offset byte. "organismCount" is the second int in args[], and 1 int = 4 bytes
		ComputeBuffer.CopyCount(organismFilteredResultBuffer, argsBuffer, 4);

		//Draw
		//3*4 is the offset byte, where the indirect draw in args starts
		Graphics.DrawProceduralIndirect(material, bounds, MeshTopology.Points, argsBuffer, 0);
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