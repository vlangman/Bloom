using System.Collections.Generic;

public partial class Globals
{

	public const int NeuronSize =
	(sizeof(int) * 3) +
	(Globals.GeneSize) +
	(Globals.ParameterSize * Globals.ParameterCount) +
	(Globals.ConnectionSize * Globals.ConnectionCount);
}


//a neuron full genome is made up from
// {NEURON TYPE,[PARAMETERS->gene]} [CONNECTIONS->gene] 
public interface Neuron : IComponent
{
	public int neuronIndex { get; set; }
	public NEURON_TYPE type { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }
	public Gene gene { get; set; }
	public List<IParameter> parameters { get; set; }

	public List<Connection> connections { get; set; }

	public void Log();
}