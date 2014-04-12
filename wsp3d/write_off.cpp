#include "write_off.h"

#include <iostream>
#include <fstream>

bool write_off(Triangulation& triangulation, std::string filename)
{
	std::ofstream file(filename,std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename  << std::endl;
		return false;
	}

	int v = triangulation.number_of_vertices();
	int f = triangulation.number_of_finite_facets();	
	int e = triangulation.number_of_finite_edges();

	file << "OFF" << std::endl;
	file << v << " " << f << " " << e << std::endl;

	for (auto vertex = triangulation.finite_vertices_begin(); vertex != triangulation.finite_vertices_end(); ++vertex)
	{
		file << vertex->point() << std::endl;
	}

	for (auto facet = triangulation.finite_facets_begin(); facet != triangulation.finite_facets_end(); ++facet)
	{
		std::vector<Vertex_handle> adjacent_vertices;
/*
		triangulation.finite_adjacent_vertices(facet, std::back_inserter(adjacent_vertices));

		file << "3 "; // shall be triangles
		for (auto v = adjacent_vertices.begin(); v != adjacent_vertices.end(); v++)
		{
			file << (*v)->info() << " ";
		}
*/
		file << std::endl;
	}

	file.close();

	return true;
}
