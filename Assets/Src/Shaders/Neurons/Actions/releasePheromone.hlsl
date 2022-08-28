#include "../iNeuron.hlsl"
#include "../../Pheromones/pheromone.hlsl"


class ReleasePheromone : INeuron{
	void Compute(inout Neuron neuron, inout Organism organism){
		// pheromoneBuffer[organism.idx].organismIndex = organism.idx;
		pheromoneBuffer[organism.idx].colour = organism.colour;
	}
};
static ReleasePheromone _releasePheromone;