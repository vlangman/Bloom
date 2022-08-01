
public struct Angle : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public float value { get; set; }



	public Parameter InitializeBase()
	{
		return new Parameter
		{
			type = PARAMETER_TYPE.ANGLE,
			value = ParameterFactory.GenerateParameterValue()
		};
	}



}