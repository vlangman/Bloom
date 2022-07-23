
public struct Weight : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public bool significantBit { get; set; }
	public float value { get; set; }
	public Gene gene { get; set; }

	public float minValue { get; set; }
	public float maxValue { get; set; }


	public void Initialize()
	{

		this.type = PARAMETER_TYPE.WEIGHT;
		this.minValue = -4;
		this.maxValue = 4;

		this = (Weight)ParameterFactory.GenerateParameter(this);
	}



}