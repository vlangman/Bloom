
public struct Polarity : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public float value { get; set; }


	public Parameter InitializeBase()
	{
		return new Parameter
		{
			type = PARAMETER_TYPE.POLARITY,
			value = ParameterFactory.GenerateParameterValue()
		};
	}



}