#include "../iNeuron.hlsl"
#include "../../Pheromones/pheromone.hlsl"


class UpdateOrganismColour : INeuron{
	void Compute(inout Neuron neuron, inout Organism organism){
		neuron.value = 0.5;
	}
};
static UpdateOrganismColour _updateOrganismAction;