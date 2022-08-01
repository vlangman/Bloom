using UnityEngine;
public static class ParameterFactory
{
	public static float GenerateParameterValue(bool rounded = false)
	{
		//generate random value for the parameter
		float value = Random.Range(-128f, 129f);
		if (rounded)
			value = Mathf.Round(value);

		//when adding gene format to 2dp (n2) for to store 2 bytes as parameter value for gene
		// parameter.gene = GeneFactory.ToGene(parameter);
		return value;
	}


}