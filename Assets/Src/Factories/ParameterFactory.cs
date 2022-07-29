// using UnityEngine;
// using System.Collections.Generic;
// public static class ParameterFactory
// {
// 	public static IParameter GenerateParameter(IParameter parameter, bool rounded = false)
// 	{
// 		//generate random value for the parameter
// 		parameter.value = Random.Range(parameter.minValue, parameter.maxValue + 1);
// 		if (rounded)
// 			parameter.value = Mathf.Round(parameter.value);
// 		parameter.significantBit = parameter.value < 0 ? true : false;
// 		parameter.gene = GeneFactory.ToGene(parameter);
// 		return parameter;
// 	}

// 	public static List<IParameter> GenerateParameters(Neuron neuron)
// 	{
// 		foreach (var parameter in neuron.parameters)
// 		{
// 			parameter.Initialize();
// 		}
// 		neuron.Log();
// 		return neuron.parameters;
// 	}

// 	public static IParameter SetParameter(IParameter parameter, float value)
// 	{
// 		parameter.value = value;
// 		parameter.significantBit = value < 0 ? true : false;
// 		parameter.gene = GeneFactory.ToGene(parameter);
// 		return parameter;
// 	}

// 	public static void OverrideParameter(PARAMETER_TYPE type, ref Neuron neuron, float value)
// 	{

// 		for (int i = 0; i < neuron.parameters.Count; i++)
// 		{
// 			if (neuron.parameters[i].type == type)
// 			{
// 				neuron.parameters[i] = SetParameter(neuron.parameters[i], value);
// 				return;
// 			}
// 		}

// 		throw new System.Exception($"Parameter {type.ToString()} not found in neuron of type {neuron.type.ToString()}");

// 	}

// }