

#include "../Parameters/Parameter.hlsl"
#include "../Organisms/organism.hlsl"

#ifndef __NEURON
#define __NEURON


	#define EMITTER 0
	#define COLLECTOR 1
	#define ACTION 2


	//emitters
	#define PERIODIC_SIGNAL 0
	#define PHEROMONE_GRID_SENSOR 1
	//actions
	#define UPDATE_ORGANISM_COLOUR 0
	#define PHEROMONE


	struct Neuron
	{
		uint neuronIndex;
		uint type;
		uint baseType;

		float value;

		Parameter parameter1;
		Parameter parameter2;
		Parameter parameter3;
		Parameter parameter4;
		
	};

	interface INeuron {
		void Compute(inout Neuron neuron, inout Organism organism);
	};


#endif