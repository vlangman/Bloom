
public struct Angle : IParameter
{

	public PARAMETER_TYPE type { get; set; }
	public bool significantBit { get; set; }
	public float value { get; set; }
	public Gene gene { get; set; }

	public float minValue { get; set; }
	public float maxValue { get; set; }


	public void Initialize()
	{

		this.type = PARAMETER_TYPE.ANGLE;
		this.minValue = -255;
		this.maxValue = 255;

		this = (Angle)ParameterFactory.GenerateParameter(this);
	}



}