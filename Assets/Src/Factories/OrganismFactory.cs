using UnityEngine;
using System.Collections.Generic;

public static class OrganismFactory
{
	public static Organism CreateSlimeBase(uint index, ref List<INeuron> neuronBuffer)
	{
		NeuronFactory neuronFactory = new NeuronFactory();
		Organism organism = new Organism();
		organism.idx = index;
		organism.position = new Vector2(0, 0);
		organism.color = new Vector4(0, 1, 1, 1);

		// var forwardLineSensor = neuronFactory.GenerateNeuron(NEURON_TYPE.POPULATION_LINE_EMITTER);
		// ParameterFactory.OverrideParameter(PARAMETER_TYPE.ANGLE, ref forwardLineSensor, 0.0f);
		// var continuousEmitter = neuronFactory.GenerateNeuron(NEURON_TYPE.CONTINUOUS_EMITTER, index);
		// var moveAction = neuronFactory.GenerateNeuron(NEURON_TYPE.MOVE_ACTION,index);
		// var TurnAction = neuronFactory.GenerateNeuron(NEURON_TYPE.TURN_ACTION,index);
		// ParameterFactory.OverrideParameter(PARAMETER_TYPE.SPEED, ref TurnAction, 10f);

		// ConnectionFactory.CreateConnection(ref continuousEmitter, ref moveAction, 1f);
		// ConnectionFactory.CreateConnection(ref continuousEmitter, ref TurnAction, 1f);
		INeuron neuron = new PeriodicEmitter();
		uint neuronIndex = (uint)neuronBuffer.Count * Globals.NeuronCount + 1;
		neuron.neuronIndex = neuronIndex;
		neuronBuffer.Add(neuron);


		organism.neuronsStartIndex = neuronIndex;

		// organism.Log();
		return organism;

	}
}