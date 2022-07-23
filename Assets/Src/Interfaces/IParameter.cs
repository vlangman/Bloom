
public partial class Globals
{

	public const int ParameterSize =
	(sizeof(int) * 1) +
	(Globals.GeneSize) +
	(sizeof(bool) * 1) +
	(sizeof(float) * 3);
}


public interface IParameter : IComponent
{
	public PARAMETER_TYPE type { get; set; }
	public Gene gene { get; set; }

	//1 if value is negative 0 if positive
	public bool significantBit { get; set; }
	//value of the parameter min 00 max FF 
	public float value { get; set; }
	//min value of parameter
	public float minValue { get; set; }
	//max value of parameter
	public float maxValue { get; set; }


}