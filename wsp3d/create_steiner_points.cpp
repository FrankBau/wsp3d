#include "create_steiner_points.h"

void create_steiner_points_for_cell(Triangulation& triangulation, Cell_handle cell)
{
	std::cerr << "create_steiner_points_for_cell(" << cell->info() << ")" << std::endl;
	Vertex_handle v0 = cell->vertex(0);
	Vertex_handle v1 = cell->vertex(1);
	Vertex_handle v2 = cell->vertex(2);
	Vertex_handle v3 = cell->vertex(3);

	// we must ensure that every facet in the triangulation is procesed exactly once
	// there are, however, no explicit facets or edges stored in a triangulation
	// so we process a facet f incident to a cell iff 
	// - the other cell containing f is the infinite cell, or
	// - the other cell containing f has a larger number than this cell

	for (int i = 0; i<4; ++i)
	{
		Cell_handle cn = cell->neighbor(i);
		if (triangulation.is_infinite(cn) || cn->info() > cell->info())
		{
			std::cout << "  create_steiner_points_for_facet(" 
				<< cell->vertex((i + 1) % 4)->info() << ","
				<< cell->vertex((i + 2) % 4)->info() << ","
				<< cell->vertex((i + 3) % 4)->info() << ")"
				<< std::endl;
		}
	}
}

/*
  add points on the facets and along the edges of triangulation
*/
void create_steiner_points(Triangulation& triangulation)
{
	for (auto facet = triangulation.finite_facets_begin(); facet != triangulation.finite_facets_end(); ++facet)
	{
		std::cout << "cell=" << facet->first->info() << ", index=" << facet->second << std::endl;
		
	}

	return;

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		create_steiner_points_for_cell(triangulation, cell);
	}
}
