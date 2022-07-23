using System;
using System.Collections.Generic;


public static class NeuronTypeGenerator
{

	public static Neuron GenerateType(NEURON_TYPE type)
	{
		switch (type)
		{
			case NEURON_TYPE.CONTINUOUS_EMITTER:
				return new ContinuousEmitter();
			case NEURON_TYPE.POPULATION_LINE_EMITTER:
				return new PopulationLineEmitter();
			case NEURON_TYPE.MOVE_ACTION:
				return new MoveAction();
			case NEURON_TYPE.TURN_ACTION:
				return new TurnAction();
			default:
				throw new NotImplementedException($"GenerateType() map for {type.ToString()} not implemented ");
		}

	}
}

public class NeuronFactory
{
	public List<Neuron> Neurons { get; set; } = new List<Neuron>();
	public List<Connection> Connections { get; set; } = new List<Connection>();

	//generate a new neuron of random NEURON_TYPE 
	//generate a new gene for the neuron with random parameters within the range of the neuron type parameters
	//add the neuron to the list of neurons
	//return the neuron
	public Neuron GenerateRandomNeuron()
	{
		int totalTypes = Enum.GetNames(typeof(NEURON_TYPE)).Length;
		int randomType = UnityEngine.Random.Range(0, totalTypes);
		Neuron neuron = NeuronTypeGenerator.GenerateType((NEURON_TYPE)randomType);
		//initialize the neurons properties
		neuron.Initialize();
		neuron.neuronIndex = Neurons.Count;
		Neurons.Add(neuron);
		return neuron;
	}

	public Neuron GenerateNeuron(NEURON_TYPE neuronType)
	{
		Neuron neuron = NeuronTypeGenerator.GenerateType(neuronType);
		//initialize the neurons properties
		neuron.Initialize();
		neuron.neuronIndex = Neurons.Count;
		Neurons.Add(neuron);
		return neuron;
	}


}