public struct UpdateColour : INeuron
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
			baseType = NEURON_BASE_TYPE.ACTION,
			type = (uint)ACTION_TYPE.UPDATE_ORGANISM_COLOUR
		};

	}

}