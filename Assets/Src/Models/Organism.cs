using UnityEngine;
using System.Collections.Generic;

public partial class Globals
{
	//position + color
	public const int OrganismSize = sizeof(float) * 2 + sizeof(float) * 4 + Globals.NeuronSize;
}

public struct Organism
{
	public Vector2 position;
	public Vector4 color;
	public List<Neuron> neurons;

	public void Log()
	{
		foreach (var neuron in neurons)
		{
			neuron.Log();
		}
	}

}