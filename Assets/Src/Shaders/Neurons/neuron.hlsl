
//include buffers here for all iNeuron children

#include "../Parameters/Parameter.hlsl"
#include "../Organisms/organism.hlsl"

#ifndef __NEURON
#define __NEURON



	//neuron types
	#define EMITTER 0
	#define COLLECTOR 1
	#define ACTION 2


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


#endif