public partial class Globals
{

	public const int ConnectionSize =
	(sizeof(float) * 1) +
	(sizeof(int) * 1) +
	(Globals.GeneSize);
}


public struct Connection
{
	public float weight;
	public int sourceIndex;
	public Gene gene;

}