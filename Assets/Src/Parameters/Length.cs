
public struct Length : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public bool significantBit { get; set; }
	public float value { get; set; }
	public Gene gene { get; set; }

	public float minValue { get; set; }
	public float maxValue { get; set; }


	public void Initialize()
	{
		this.type = PARAMETER_TYPE.LENGTH;
		this.minValue = 10;
		this.maxValue = 255;

		// this = (Length)ParameterFactory.GenerateParameter(this);
	}



}