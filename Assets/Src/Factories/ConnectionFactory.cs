using System;

public static class ConnectionFactory
{
	public static Connection CreateConnection(ref Neuron from, ref Neuron to, float weight)
	{
		//self reference need to set itself as a in connection
		if (from == to)
		{
			throw new NotImplementedException("Self reference not implemented");
		}

		Connection connection = new Connection
		{
			sourceIndex = from.neuronIndex,
			weight = weight,
		};
		if (to.connections == null)
			throw new Exception($"{to.type.ToString()} connections not instantiated on Initialize()");
		connection.gene = GeneFactory.ToGene(connection);
		to.connections.Add(connection);
		return connection;
	}
}