#include "../buffers.hlsl"

#ifndef __INEURON
#define __INEURON


//inherited neuron types

//emitters
#define PERIODIC_SIGNAL 0
#define PHEROMONE_GRID_SENSOR 1
//actions
#define UPDATE_ORGANISM_COLOUR 0
#define RELEASE_PHEROMONE 1


interface INeuron {
	void Compute(inout Neuron neuron, inout Organism organism);
};

#endif