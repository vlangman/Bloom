
#include "../globals.hlsl" 
#include "../Parameters/Parameter.hlsl"

#ifndef __NEURON
#define __NEURON


// NEURON_BASE_TYPE
	#define EMITTER 0
	#define COLLECTOR 1
	#define ACTION 2

	#define NEURON_BASE_TYPE_LIST_SIZE ACTION+1
//end NEURON_BASE_TYPE


// NEURON_TYPE
	//emitters
	#define PERIODIC_EMITTER 0
	//actions
	#define UPDATE_COLOUR_ACTION 1

	#define NEURON_TYPE_LIST_SIZE UPDATE_COLOUR_ACTION+1
//end NEURON_TYPE



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
	Neuron Compute(Neuron neuron);
};


#endif // __NEURON