using UnityEngine;
public struct PeriodicSignal : INeuron
{
	public uint neuronIndex { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }
	public uint type { get; set; }

	public Parameter parameter1 { get; set; }
	public Parameter parameter2 { get; set; }
	public Parameter parameter3 { get; set; }
	public Parameter parameter4 { get; set; }


	public Neuron InitializeBase()
	{
		return new Neuron
		{
			baseType = NEURON_BASE_TYPE.EMITTER,
			type = (uint)EMITTER_TYPE.PERIODIC_SIGNAL,

			parameter1 = new Parameter
			{
				//used to store the period/frequency of the emitter
				type = PARAMETER_TYPE.TIME,
				value = 0.00001f
			},
			parameter2 = new Parameter
			{
				//used to store the elapsed time for calculating if period has elapsed
				type = PARAMETER_TYPE.TIME,
				value = 0.0f
			},
			parameter3 = new Parameter
			{
				//used to store the duration of the pulse 
				type = PARAMETER_TYPE.TIME,
				value = 1.0f
			},
			parameter4 = new Parameter
			{
				//used to store the duration of the pulse 
				type = PARAMETER_TYPE.STRENGTH,
				value = 0.5f
			}

		};

	}

}