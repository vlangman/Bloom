
public struct PheromoneGridSensor : INeuron
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
			type = (uint)EMITTER_TYPE.PHEROMONE_GRID_SENSOR,

			parameter1 = new Parameter
			{
				//used to store the angle from 0 
				type = PARAMETER_TYPE.ANGLE,
				value = 0.00001f
			},
			parameter2 = new Parameter
			{
				// length to travel in direction ANGLE
				type = PARAMETER_TYPE.LENGTH,
				value = 0.0f
			},
			parameter3 = new Parameter
			{
				//used to store gridSize detected 
				type = PARAMETER_TYPE.SIZE,
				value = 1.0f
			},

		};

	}

}