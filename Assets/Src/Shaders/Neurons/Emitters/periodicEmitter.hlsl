#include "../neuron.hlsl"

class PeriodicEmitter : INeuron{
	Neuron Compute(Neuron neuron){
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
			}
		}
		
		neuron.parameter2.value += programState._deltaTime;
		return neuron;
	}
};
static PeriodicEmitter _periodicEmitter;