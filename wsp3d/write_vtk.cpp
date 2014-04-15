#include "write_vtk.h"

/*
This will write the triangulation to VTK file
*/
bool write_vtk(Triangulation& triangulation, std::string filename)
{
	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename << std::endl;
		return false;
	}

	int v = triangulation.number_of_vertices();
	int c = triangulation.number_of_finite_cells();

	file <<
		"# vtk DataFile Version 2.0\n"
		"tetrahedralization\n"
		"ASCII\n"
		"DATASET UNSTRUCTURED_GRID\n";

	file << "POINTS " << v << " double\n";
	for (auto vertex = triangulation.finite_vertices_begin(); vertex != triangulation.finite_vertices_end(); ++vertex)
	{
		file << vertex->point() << "\n";
	}

	// for each cell 5 values are given: numPoints (4) and the vertex indices
	file << "CELLS " << c << " " << 5 * c << "\n";
	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		file << "4 ";
		for (int i = 0; i < 4; ++i)
		{
			file << cell->vertex(i)->info() << " ";
		}
		file << "\n";
	}

	file << "CELL_TYPES " << c << "\n";
	for (int i = 0; i < c; ++i)
	{
		// vtk cell type 10 is tetrahedron
		file << "10" "\n";
	}
	file
		<< "CELL_DATA " << c << "\n"
		<< "SCALARS weight double 1\n"
		<< "LOOKUP_TABLE default\n";

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		file << cell->weight();
		file << "\n";
	}

	file.close();
}

bool write_shortest_path_vtk(Graph& graph, std::vector<GraphNode_descriptor>& predecessors, std::string filename)
{
	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename << std::endl;
		return false;
	}

	int n = boost::num_vertices(graph);

	file <<
		"# vtk DataFile Version 2.0\n"
		"shortest path\n"
		"ASCII\n"
		"DATASET UNSTRUCTURED_GRID\n";
	file << "POINTS " << n << " double\n";

	std::vector<int> id(num_vertices(graph));
	int i = 0;
	Graph::vertex_iterator vertexIt, vertexEnd;
	for ( boost::tie(vertexIt, vertexEnd) = boost::vertices(graph); vertexIt != vertexEnd; ++vertexIt )
	{
		GraphNode_descriptor v = *vertexIt;
		id[v] = i++;
		file << graph[v].point << "\n";
	}

	file << "CELLS " << n << " " << 3 * n << "\n";
	for (boost::tie(vertexIt, vertexEnd) = boost::vertices(graph); vertexIt != vertexEnd; ++vertexIt)
	{
		GraphNode_descriptor u = *vertexIt;
		GraphNode_descriptor v = predecessors[u];
		file << "2 " << id[u] << " " << id[v] << "\n";
	}
	file << "\n";

	file << "CELL_TYPES " << n << "\n";
	for (int i = 0; i < n; ++i)
	{
		// vtk cell type 3 is line
		file << "3" "\n";
	}

}
