#include "../Organisms/organism.hlsl"

// emitters
#include "Emitters/periodicSignal.hlsl"
//actions
#include "Actions/updateOrganismColour.hlsl"


#ifndef __NEURON_LOOKUP
#define __NEURON_LOOKUP


void ProcessNeuron(inout Neuron neuron, inout Organism organism)
{
	if(EMITTER == neuron.baseType)
	{
		switch(neuron.type){
			case PERIODIC_SIGNAL : _periodicSignal.Compute(neuron,organism); break;		
			default: break;
		};
	}
	else if(ACTION == neuron.baseType)
	{
		switch(neuron.type){
			case UPDATE_ORGANISM_COLOUR: _updateOrganismAction.Compute(neuron,organism); break;
			default: break;
		};
	}
}


#endif