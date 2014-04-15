#include "common.h"
#include "create_tetrahedralization.h"

void set_cell_and_vertex_ids(Triangulation& triangulation)
{
	triangulation.infinite_cell()->info() = -1;
	triangulation.infinite_vertex()->info() = -1;

	int i = 0;
	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		cell->info() = i++;
	}

	// assign number to vertices, starting at 0
	i = 0;
	for (auto vertex = triangulation.finite_vertices_begin(); vertex != triangulation.finite_vertices_end(); ++vertex)
	{
		vertex->info() = i++;
	}
}

void set_random_weights(Triangulation& triangulation)
{
	triangulation.infinite_cell()->weight() = std::numeric_limits<double>::max();

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		cell->weight() = CGAL::default_random.get_double(0,1); // random ?
	}
}

void propagate_weights(Triangulation& triangulation)
{
	// calc weights of faces 
	for (auto cell = triangulation.cells_begin(); cell != triangulation.cells_end(); ++cell)
	{
		Cell_handle c = cell;
		double wc = c->weight();
		for (int i = 0; i < 4; ++i)
		{
			Cell_handle cn = c->neighbor(i);
			int j = triangulation.mirror_index(c, i);
			assert(cn->neighbor(j) == c);

			double wn = cn->weight();
			double wf = std::min(wc, wn);
			c->weight(i) = wf;  // weight of Facet(c,i)
		}
	}

	// calc weights of edges
	for (auto edge = triangulation.edges_begin(); edge != triangulation.edges_end(); ++edge)
	{
		double weight = std::numeric_limits<double>::max();
		Facet_circulator facet_start = triangulation.incident_facets(*edge);
		auto facet = facet_start;
		do
		{
			Cell_handle cell = facet->first;
			int i = facet->second;
			weight = std::min(weight, cell->weight(i));
		} while (++facet != facet_start);

		Cell_handle cell = edge->first;
		int i = edge->second;
		int j = edge->third;
		cell->weight(i, j) = weight;
		// cell->weight(j, i) = weight; // is this really needed?
	}
}

/*
  creates a n x m x k array of tetrahedralized cubes 
*/
void create_cubes(Triangulation& triangulation, int n, int m, int k)
{
	std::vector<Point> pointVector;
	int number_of_points = 0;
	for (int z = 0; z<n + 1; z++)
	{
		for (int y = 0; y<m + 1; y++)
		{
			for (int x = 0; x<k + 1; x++)
			{
				Point point(x, y, z);
				pointVector.push_back(point);
				++number_of_points;
			}
		}
	}

	// insert all points in a row (this is faster than one insert() at a time).
	triangulation.insert(pointVector.begin(), pointVector.end());
	assert(triangulation.is_valid());
	assert(triangulation.dimension() == 3);
}