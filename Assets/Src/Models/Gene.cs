

public partial class Globals
{
	public const int GeneSize =
	(sizeof(int) * 1) +
	(sizeof(float) * 1) +
	(16) * 2;
}

public struct Gene
{
	public GENE_TYPE type { get; set; }
	public float value { get; set; }
	public string hex { get; set; }
	public string binary { get; set; }

}