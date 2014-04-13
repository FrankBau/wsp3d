#include "create_steiner_points.h"

void create_steiner_points_for_cell(Triangulation& triangulation, Cell_handle cell)
{
	std::cerr << "create_steiner_points_for_cell(" << cell->info() << ")";

	Point p0 = cell->vertex(0)->point();
	Point p1 = cell->vertex(1)->point();
	Point p2 = cell->vertex(2)->point();
	Point p3 = cell->vertex(3)->point();

	Kernel::Vector_3 v0 = p0 - CGAL::ORIGIN;
	Kernel::Vector_3 v1 = p1 - CGAL::ORIGIN;
	Kernel::Vector_3 v2 = p2 - CGAL::ORIGIN;
	Kernel::Vector_3 v3 = p3 - CGAL::ORIGIN;

	Kernel::Vector_3 v = ( v0 + v1 + v2 + v3 ) / 2;

	Point p = CGAL::ORIGIN + v;

	std::cerr << " center point at [ " << p << " ] " << std::endl;

	return;

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
void create_steiner_points(Graph& graph, Triangulation& triangulation)
{
	for (auto facet = triangulation.finite_facets_begin(); facet != triangulation.finite_facets_end(); ++facet)
	{
		std::cout << "cell=" << facet->first->info() << ", index=" << facet->second << std::endl;
		
	}

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		create_steiner_points_for_cell(triangulation, cell);
	}
}
