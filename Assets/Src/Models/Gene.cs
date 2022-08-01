

public partial class Globals
{
	public const int GeneSize =
		//type
		UINT_SIZE +
		//binary
		BYTE_SIZE;

}

public struct Gene
{
	public GENE_TYPE type { get; set; }
	public byte binary { get; set; }

}