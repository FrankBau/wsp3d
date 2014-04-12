#include "dump_triangulation.h"

void dump_cells(Triangulation& triangulation)
{
	std::cout << "cells of triangulation:" << std::endl;
	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); cell++)
	{
		std::cout << cell->info() << ": ";

		std::cout << "vertices: ";
		for (int i = 0; i < 4; i++)
		{
			std::cout << cell->vertex(i)->info() << ", ";
		}

		std::cout << "neighbors: ";
		for (int i = 0; i < 4; i++)
		{
			if (triangulation.is_infinite(cell->neighbor(i)))
			{
				std::cout << "-, ";
			}
			else
			{
				std::cout << cell->neighbor(i)->info() << ", ";
			}
		}
		std::cout << "weight = " << cell->weight();
		std::cout << std::endl;
	}
}

void dump_facets(Triangulation& triangulation)
{
	std::cout << "facets of triangulation:" << std::endl;
	for (auto facet = triangulation.finite_facets_begin(); facet != triangulation.finite_facets_end(); facet++)
	{
		Cell_handle cell = facet->first;
		int i = facet->second;
		std::cout
			<< "Facet(" << cell->info() << "," << cell->vertex(i)->info() << ") ";
			// << "[ " << triangulation.triangle(*facet) << " ] "
		
		std::cout << "vertices: ";
		for (int j = 0; j < 4; ++j)
		{
			if (i == j)
				continue; // dont take the vertex opposite to facet
			std::cout << cell->vertex(j)->info() << ", ";
		}
		std::cout 
			<< "weigth = " << cell->weight(i)
			<< std::endl;
	}
}

void dump_vertices(Triangulation& triangulation)
{
	std::cout << "vertices of triangulation:" << std::endl;
	for (auto vertex = triangulation.finite_vertices_begin(); vertex != triangulation.finite_vertices_end(); vertex++)
	{
		std::cout
			<< "vertex "
			<< vertex->info()
			<< " at [ " << vertex->point() << " ] "
			<< " is adjacent to vertices: ";
		std::vector<Vertex_handle> neighbour_vertices;
		triangulation.finite_adjacent_vertices(vertex, std::back_inserter(neighbour_vertices));
		for (auto v = neighbour_vertices.begin(); v != neighbour_vertices.end(); v++)
		{
			std::cout << (*v)->info() << ", ";
		}
		std::cout << std::endl;
	}
}

void dump_edges(Triangulation& triangulation)
{
	std::cout << "edges of triangulation:" << std::endl;
	for (auto edge = triangulation.finite_edges_begin(); edge != triangulation.finite_edges_end(); edge++)
	{
		Cell_handle cell = edge->first;
		int i = edge->second;
		int j = edge->third;
		std::cout 
			<< "Edge(" << cell->info() << "," << i << "," << j << ")"
			<< "vertices: " 
			<< cell->vertex(i)->info() << ", "
			<< cell->vertex(j)->info() 
			<< " at [" << triangulation.segment(*edge) << "]"
			<< " weight " << cell->weight(i,j)
			<< std::endl;
	}
}

void dump_triangulation(Triangulation& triangulation)
{
	std::cout << "Number of finite vertices : " << triangulation.number_of_vertices() << std::endl;
	std::cout << "Number of finite edges    : " << triangulation.number_of_finite_edges() << std::endl;
	std::cout << "Number of finite facets   : " << triangulation.number_of_finite_facets() << std::endl;
	std::cout << "Number of finite cells    : " << triangulation.number_of_finite_cells() << std::endl;

	dump_vertices(triangulation);
	dump_cells(triangulation);
	dump_facets(triangulation);
	dump_edges(triangulation);
}
