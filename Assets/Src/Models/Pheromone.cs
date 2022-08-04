using UnityEngine;

public partial class Globals
{
	public const int PheromoneSize =
		// index +   
		VECTOR4_SIZE +
		// type
		UINT_SIZE;

}

public struct Pheromone
{

	public Vector4 colour;
	public uint organismIndex;

}