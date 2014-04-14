#include "write_graph_dot.h"

struct graph_property_writer 
{
	void operator()(std::ostream& out) const 
	{
		out << "graph [bgcolor=lightgrey]" << std::endl;
		out << "node [shape=circle color=white]" << std::endl;
		out << "edge [style=dashed]" << std::endl;
	}
};

template <class GraphType>
class myNodeWriter {
public:
	myNodeWriter(GraphType _graph) : graph(_graph) {}
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& v) const
	{
		out << "[label=\"" << graph[v].cell->info() << "\"]";
	}
private:
	GraphType graph;
};

template <class GraphType>
class myEdgeWriter {
public:
	myEdgeWriter(GraphType _graph) : graph(_graph) {}
	template <class VertexOrEdge>
	void operator()(std::ostream& out, const VertexOrEdge& e) const
	{
		out << "[label=\"" << graph[e].weight << "\"]";
	}
private:
	GraphType graph;
};

bool write_graph_dot(std::string filename, Graph graph)
{
	myNodeWriter<Graph> node_writer(graph);
	myEdgeWriter<Graph> edge_writer(graph);

	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "failed to open graph.dot file " << std::endl;
		return false;
	}
	boost::write_graphviz(file, graph, node_writer, edge_writer );

	// post process with dot.exe -Tsvg -O graph.dot or thelike
	file.close();

	return true;
}
