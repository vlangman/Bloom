using System.Collections.Generic;

public struct MoveAction : Neuron
{
	public int neuronIndex { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }
	public NEURON_TYPE type { get; set; }
	public Gene gene { get; set; }
	public List<Connection> connections { get; set; }
	public List<IParameter> parameters { get; set; }

	public void Log()
	{
		Logger.Log(this);
	}

	public void Initialize()
	{
		//configure this neuron
		this.baseType = NEURON_BASE_TYPE.ACTION;
		this.type = NEURON_TYPE.MOVE_ACTION;
		this.gene = GeneFactory.ToGene(this);
		this.connections = new();
		//specify and construct parameters for this neuron
		this.parameters = new List<IParameter>{
			new Speed(),
		};
		this.parameters = ParameterFactory.GenerateParameters(this);

	}

}