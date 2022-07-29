public struct ContinuousEmitter
{
	public uint neuronIndex { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }
	public NEURON_TYPE type { get; set; }
	// public Gene gene { get; set; }
	// public List<Connection> connections { get; set; }
	// public List<IParameter> parameters { get; set; }

	public void Initialize()
	{
		// configure this neuron
		this.baseType = NEURON_BASE_TYPE.EMITTER;
		this.type = NEURON_TYPE.CONTINUOUS_EMITTER;
		// this.gene = GeneFactory.ToGene(this);
		// this.connections = new();
		// this.parameters = new();
	}

}