
public partial class Globals
{

	public const int ParameterSize =
		UINT_SIZE +
		FLOAT_SIZE;
}

//inheritable implementation , component that must return an initialized base type T for copy to GPU
public interface IParameter : IComponent<Parameter>
{
	public PARAMETER_TYPE type { get; set; }
	//value of the parameter min 00 max FF 
	public float value { get; set; }

}


//concrete implementation for GPU copy
public struct Parameter
{
	public PARAMETER_TYPE type { get; set; }
	//value of the parameter min 00 max FF 
	public float value { get; set; }

}