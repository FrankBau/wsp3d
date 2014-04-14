#include "create_steiner_points.h"

void create_steiner_points_for_cell(Graph& graph, Triangulation& triangulation, Cell_handle cell)
{
	// std::cout << "create_steiner_points_for_cell(" << cell->info() << ")";

	Tetrahedron tetrahedron = triangulation.tetrahedron(cell);

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

	// std::cerr << " center point at [ " << p << " ] " << std::endl;

	GraphNode_descriptor node = boost::add_vertex(graph);
	graph[node].cell = cell;
	graph[node].point = p;

	cell->node() = node;

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

void connect_steiner_points_for_cell(Graph& graph, Triangulation& triangulation, Cell_handle cell)
{
	GraphNode_descriptor u = cell->node();

	for (int i = 0; i < 4; ++i)
	{
		Cell_handle cell_neighbour = cell->neighbor(i);
		if (cell_neighbour->info() > cell->info())
		{
			// Facet facet = Facet(cell, i); // facet opposite to vertex i 
			// Triangle triangle = triangulation.triangle(facet);
			// the 3 corner points of the triangle are
			Point p1 = cell->vertex((i + 1) % 4)->point();
			Point p2 = cell->vertex((i + 2) % 4)->point();
			Point p3 = cell->vertex((i + 3) % 4)->point();
			Kernel::Plane_3 plane(p1, p2, p3);

			GraphNode_descriptor v = cell_neighbour->node();
			Point x = graph[u].point;
			Point y = graph[v].point;
			Kernel::Line_3 line(x, y);

			// C++11 auto comes handy here because result can be a point or a line!
			auto intersection = CGAL::intersection(plane, line);
			// may throw an exception when they don't intersect in a point...
			Point z = CGAL::object_cast<Point>(intersection);
		
			double distance_xz = sqrt(CGAL::squared_distance(x, z));
			double distance_zy = sqrt(CGAL::squared_distance(y, z));

			double edge_weight = distance_xz*cell->weight() + distance_zy*cell_neighbour->weight();

#if 0
			std::cout 
				<< "connect cells " 
				<< cell->info() 
				<< " and " 
				<< cell_neighbour->info() 
				<< " meeting at point [ "
				<< z
				<< " ] with edge of weight "
				<< edge_weight
				<< std::endl;
#endif

			GraphEdge_descriptor edge;
			bool inserted;
			boost::tie(edge, inserted) = boost::add_edge(u, v, graph);
			assert(inserted);
			graph[edge].weight = edge_weight;
		}
	}
}

/*
  add points on the facets and along the edges of triangulation
*/
void create_steiner_points(Graph& graph, Triangulation& triangulation)
{
	//for (auto facet = triangulation.finite_facets_begin(); facet != triangulation.finite_facets_end(); ++facet)
	//{
	//	std::cout << "cell=" << facet->first->info() << ", index=" << facet->second << std::endl;
	//	
	//}

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		create_steiner_points_for_cell(graph, triangulation, cell);
	}

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		connect_steiner_points_for_cell(graph, triangulation, cell);
	}
}
