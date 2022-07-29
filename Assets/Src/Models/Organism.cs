using UnityEngine;
using System.Collections.Generic;

public partial class Globals
{

	public const int OrganismSize =
			//idx
			sizeof(uint) +
			// alive
			UINT_SIZE +
			// orientation
			VECTOR2_SIZE +
			//position
			VECTOR2_SIZE +
			//color
			VECTOR4_SIZE +
			 //neurons
			 UINT_SIZE;
}


//The stride passed when constructing the buffer must match structure size, be a multiple of 4 and less than 2048
public struct Organism
{

	public uint idx;//4 bytes 

	public uint alive;//4  bytes

	public Vector2 orientation;//8  bytes

	public Vector2 position;//8  bytes

	public Color color;// 16 bytes: (4*4)

	public uint neuronsStartIndex;// 4 bytes

}


