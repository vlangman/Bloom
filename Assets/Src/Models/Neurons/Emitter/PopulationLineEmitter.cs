// using System.Collections.Generic;

// public struct PopulationLineEmitter : Neuron
// {
// 	public uint neuronIndex { get; set; }
// 	public NEURON_BASE_TYPE baseType { get; set; }
// 	public NEURON_TYPE type { get; set; }
// 	public Gene gene { get; set; }
// 	public List<Connection> connections { get; set; }
// 	public List<IParameter> parameters { get; set; }

// 	public void Log()
// 	{
// 		// Logger.Log(this);
// 	}

// 	public void Initialize()
// 	{
// 		//configure this neuron
// 		this.baseType = NEURON_BASE_TYPE.EMITTER;
// 		this.type = NEURON_TYPE.POPULATION_LINE_EMITTER;
// 		this.gene = GeneFactory.ToGene(this);
// 		this.connections = new();
// 		//specify and construct parameters for this neuron
// 		this.parameters = new List<IParameter>{
// 			new Length(),
// 			new Angle(),
// 		};
// 		// this.parameters = ParameterFactory.GenerateParameters(this);

// 	}

// }