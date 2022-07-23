
using System;
using UnityEngine;

public static class GeneFactory
{

	//neuron genes is made up from 
	//{NEURON TYPE,PARAMETERS,CONNECTION}

	private const string GENE_KEYS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	//convert a parameter to a gene and return it
	//parameters hex and binary are proceeded by the signed bit
	public static Gene ToGene(IParameter parameter)
	{
		Gene gene = new Gene
		{
			type = GENE_TYPE.PARAMETER,
			value = parameter.value,
			hex = $"[{significantBit(parameter.significantBit)}]{ToHex(parameter.value)}",
			binary = $"[{significantBit(parameter.significantBit)}]{ToBinary(parameter.value)}"
		};

		return gene;
	}

	//convert a connection to a gene and return it
	public static Gene ToGene(Connection connection)
	{
		Gene gene = new Gene
		{
			type = GENE_TYPE.CONNECTION,
			value = connection.weight,
			hex = $"{ToHex(connection.sourceIndex)}[{significantBit(connection.weight < 0 ? true : false)}]{ToHex(connection.weight)}",
			binary = $"{ToHex(connection.sourceIndex)}[{significantBit(connection.weight < 0 ? true : false)}]{ToBinary(connection.weight)}"
		};

		return gene;
	}

	//convert a neuron to a gene and return it
	public static Gene ToGene(Neuron neuron)
	{
		Gene gene = new Gene
		{
			type = GENE_TYPE.NEURON,
			value = (float)neuron.type,
			hex = ToHex((int)neuron.type),
			binary = ToBinary((int)neuron.type)
		};
		return gene;
	}

	public static string significantBit(bool significantBit)
	{
		if (significantBit)
		{
			return "1";
		}
		else
		{
			return "0";
		}
	}


	public static string ToHex(float value)
	{
		value = Math.Abs(value);
		string intString = value.ToString("n2");
		//convert mantissa and exponent integers to hex so we have something that looks like {MANTISSA X EXPONENT}-> {FF X FF}
		int commaIndex = intString.IndexOf(",");
		string mantissa = ToHex(Convert.ToInt32(intString.Substring(0, commaIndex)));
		string exponent = ToHex(Convert.ToInt32(intString.Substring(commaIndex + 1, intString.Length - 1 - commaIndex)));
		//create hex string
		string hexString = mantissa + "x" + exponent;

		// Debug.Log($"{intString} -> {hexString}");
		return hexString;
	}

	public static string ToHex(int value)
	{
		return Pad(value.ToString("X"), 2, '0');
	}

	public static string ToBinary(float value)
	{
		value = Math.Abs(value);
		string intString = value.ToString("n2");
		//convert mantissa and exponent integers to hex so we have something that looks like {MANTISSA ' ' EXPONENT}-> {[signedBit]0000000 ' ' 0000000}
		int commaIndex = intString.IndexOf(",");
		string mantissa = ToBinary(Convert.ToInt32(intString.Substring(0, commaIndex)));
		string exponent = ToBinary(Convert.ToInt32(intString.Substring(commaIndex + 1, intString.Length - 1 - commaIndex)));
		return mantissa + " " + exponent;
	}

	public static string ToBinary(int value)
	{
		return Pad(Convert.ToString(value, 2), 8, '0');
	}

	public static string Pad(string value, int desiredSize, char paddingChar)
	{

		if (value.Length < desiredSize)
			return value.PadLeft(desiredSize, paddingChar);
		else
			return value;

	}



}

