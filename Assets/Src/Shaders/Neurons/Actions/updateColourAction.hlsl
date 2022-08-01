#include "../neuron.hlsl"

class UpdateColourAction : INeuron{
	Neuron Compute(Neuron neuron){
		neuron.value = 0.5;
		return neuron;
	}
};
static UpdateColourAction _updateColourAction;