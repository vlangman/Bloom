using UnityEngine;
using System.Collections.Generic;

public static class OrganismFactory
{
	public static Organism CreateSlimeBase()
	{
		NeuronFactory neuronFactory = new NeuronFactory();
		Organism organism = new Organism();
		organism.position = new Vector2(0, 0);
		organism.color = new Vector4(1, 1, 1, 1);
		organism.neurons = new List<Neuron>();

		// var forwardLineSensor = neuronFactory.GenerateNeuron(NEURON_TYPE.POPULATION_LINE_EMITTER);
		// ParameterFactory.OverrideParameter(PARAMETER_TYPE.ANGLE, ref forwardLineSensor, 0.0f);
		var continuousEmitter = neuronFactory.GenerateNeuron(NEURON_TYPE.CONTINUOUS_EMITTER);
		var moveAction = neuronFactory.GenerateNeuron(NEURON_TYPE.MOVE_ACTION);
		var TurnAction = neuronFactory.GenerateNeuron(NEURON_TYPE.TURN_ACTION);
		ParameterFactory.OverrideParameter(PARAMETER_TYPE.SPEED, ref TurnAction, 10f);

		ConnectionFactory.CreateConnection(ref continuousEmitter, ref moveAction, 1f);
		ConnectionFactory.CreateConnection(ref continuousEmitter, ref TurnAction, 1f);

		organism.neurons.Add(continuousEmitter);
		organism.neurons.Add(TurnAction);
		organism.neurons.Add(moveAction);

		organism.Log();
		return organism;

	}
}