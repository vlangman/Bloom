using UnityEngine;

public static class Logger
{
	public static void Log(Neuron neuron)
	{
		string info = $"{neuron.type.ToString()} \n";
		foreach (IParameter parameter in neuron.parameters)
		{
			info += $"- {parameter.type.ToString()} \n\t";
			info += GeneLog(parameter.gene);
		}
		info += "\n";
		info += ConnectionLog(neuron);
		Debug.Log(info);
	}

	public static string ConnectionLog(Neuron neuron)
	{
		string info = "";
		foreach (Connection connection in neuron.connections)
		{
			info += GeneLog(connection.gene);
		}
		return info;
	}

	public static void Log(Gene gene)
	{
		Debug.Log(GeneLog(gene));
	}

	private static string GeneLog(Gene gene)
	{
		string info = $"GENE:({gene.type.ToString()}) \t [{gene.value} : {gene.hex} : {gene.binary}]\n";
		return info;
	}
}