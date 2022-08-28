#include "../iNeuron.hlsl"
#include "../../globals.hlsl"

class PeriodicSignal : INeuron{
	void Compute(inout Neuron neuron, inout Organism organism){
		//param 1 is period
		float period = neuron.parameter1.value;
		// param2 is internalTimeElapsed
		float elapsedTime = neuron.parameter2.value;
		float duration = neuron.parameter3.value;
		float strength = neuron.parameter4.value;

		if(elapsedTime >= period)
		{
			if(elapsedTime >= period + duration)
			{
				neuron.parameter2.value = 0.0;
				neuron.value = 0;
			}
			else{
				neuron.value = strength;
				organism.colour = float4(1,1,1,1);
			}
		}else{
				organism.colour = float4(1,1,1,0);
		}
		
		neuron.parameter2.value += programState._deltaTime;
	}
};
static PeriodicSignal _periodicSignal;