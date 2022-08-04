using System;
using System.Collections.Generic;


public static class NeuronTypeGenerator
{

	public static INeuron GenerateType(NEURON_BASE_TYPE type)
	{
		switch (type)
		{
			// case NEURON_TYPE.CONTINUOUS_EMITTER:
			// 	return newPeriodicSignal();
			// case NEURON_TYPE.POPULATION_LINE_EMITTER:
			// 	return new PopulationLineEmitter();
			// case NEURON_TYPE.MOVE_ACTION:
			// 	return new MoveAction();
			// case NEURON_TYPE.TURN_ACTION:
			// 	return new TurnAction();
			default:
				throw new NotImplementedException($"GenerateType() map for {type.ToString()} not implemented ");
		}

	}
}

public class NeuronFactory
{
	public List<INeuron> Neurons { get; set; } = new List<INeuron>();
	public List<Connection> Connections { get; set; } = new List<Connection>();

	//generate a new neuron of random NEURON_TYPE 
	//generate a new gene for the neuron with random parameters within the range of the neuron type parameters
	//add the neuron to the list of neurons
	//return the neuron
	// public INeuron GenerateRandomNeuron(uint index)
	// {
	// 	int totalTypes = Enum.GetNames(typeof(EMITTER_TYPE)).Length + Enum.GetNames(typeof(ACTION_TYPE)).Length;
	// 	int randomType = UnityEngine.Random.Range(0, totalTypes);
	// 	INeuron neuron = NeuronTypeGenerator.GenerateType((NEURON_BASE_TYPE)randomType);
	// 	//initialize the neurons properties
	// 	neuron.neuronIndex = index;
	// 	Neurons.Add(neuron);
	// 	return neuron;
	// }

	// public INeuron GenerateNeuron(NEURON_TYPE neuronType, uint index)
	// {
	// 	INeuron neuron = NeuronTypeGenerator.GenerateType(neuronType);
	// 	//initialize the neurons properties
	// 	neuron.neuronIndex = index;
	// 	Neurons.Add(neuron);
	// 	return neuron;
	// }


}