
public struct Speed : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public bool significantBit { get; set; }
	public float value { get; set; }
	public Gene gene { get; set; }

	public float minValue { get; set; }
	public float maxValue { get; set; }


	public void Initialize()
	{
		this.type = PARAMETER_TYPE.SPEED;
		this.minValue = 1;
		this.maxValue = 40;

		// this = (Speed)ParameterFactory.GenerateParameter(this);
	}



}