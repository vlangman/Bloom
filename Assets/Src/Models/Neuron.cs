public partial class Globals
{
	public const int NeuronSize =
		// index +   
		UINT_SIZE +
		// type
		UINT_SIZE +
		//baseType
		UINT_SIZE +
		//value
		FLOAT_SIZE +
		//parameters [1-4]
		ParameterSize * 4;

}
//abstract implementation for all overloaded neurons to inherit from 
public interface INeuron : IComponent<Neuron>
{
	public uint neuronIndex { get; set; }
	public uint type { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }

	public Parameter parameter1 { get; set; }
	public Parameter parameter2 { get; set; }
	public Parameter parameter3 { get; set; }
	public Parameter parameter4 { get; set; }
}

//concrete implementation where overloaded data will be copied
// separation here because Interfaces (INeuron) to not produce a blittable array to copy into GPU 
public struct Neuron
{

	public uint neuronIndex { get; set; }
	public uint type { get; set; }
	public NEURON_BASE_TYPE baseType { get; set; }
	public float value { get; set; }


	//blittable types cant include sub arrays so this will have to do ¯\_(ツ)_/¯
	public Parameter parameter1 { get; set; }
	public Parameter parameter2 { get; set; }
	public Parameter parameter3 { get; set; }
	public Parameter parameter4 { get; set; }

}
