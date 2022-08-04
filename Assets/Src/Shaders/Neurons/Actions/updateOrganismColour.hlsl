#include "../neuron.hlsl"

class UpdateOrganismAction : INeuron{
	void Compute(inout Neuron neuron, inout Organism organism){
		neuron.value = 0.5;
	}
};
static UpdateOrganismAction _updateOrganismAction;