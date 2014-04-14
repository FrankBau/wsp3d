#include "common.h"

#include "create_tetrahesdralization.h"
#include "dump_triangulation.h"
#include "create_steiner_points.h"

#include "read_off.h"
#include "write_off.h"
#include "write_vrml.h"
#include "write_graph_dot.h"

#include <iostream>
#include <fstream>

void set_info_and_weights(Triangulation& triangulation)
{
	triangulation.infinite_cell()->info() = 0;
	triangulation.infinite_vertex()->info() = 0;

	triangulation.infinite_cell()->weight() = std::numeric_limits<double>::max();

	// set cell weights and 
	// assign number to cells, starting at 1
	int i = 1;
	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		// cell->weight() = CGAL::default_random.get_double(0,1); // random ?
		cell->weight() = i; // random ?
		cell->info() = i++;
	}

	// assign number to vertices, starting at 1
	i = 1;
	for (auto vertex = triangulation.finite_vertices_begin(); vertex != triangulation.finite_vertices_end(); ++vertex)
	{
		vertex->info() = i++;
	}

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

int main()
{
	Triangulation triangulation;
	
	create_cubes(triangulation, 50, 50, 50 );

	// read_off(triangulation, "C:/Carleton/Meshes/holmes_off/geometry/octahedron.off"); 
	//read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/cube.off");
	//read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/ellipsoid.off");

	std::cout << "Number of finite vertices : " << triangulation.number_of_vertices() << std::endl;
	std::cout << "Number of finite edges    : " << triangulation.number_of_finite_edges() << std::endl;
	std::cout << "Number of finite facets   : " << triangulation.number_of_finite_facets() << std::endl;
	std::cout << "Number of finite cells    : " << triangulation.number_of_finite_cells() << std::endl;

	set_info_and_weights(triangulation);

	if (triangulation.number_of_finite_cells() < 100)
	{
		dump_triangulation(triangulation);
	}

	Graph graph;

	create_steiner_points(graph,triangulation);

	// the distances are temporary, so we choose an external property for that
	std::vector<double> distances(num_vertices(graph));
	std::vector<GraphNode_descriptor> predecessors(num_vertices(graph));

	boost::dijkstra_shortest_paths(
		graph, 
		*vertices(graph).first, 
		boost::weight_map(get(&GraphEdge::weight, graph)).
		distance_map(boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, graph))).
		predecessor_map(boost::make_iterator_property_map(predecessors.begin(), get(boost::vertex_index, graph)))
	);
	
	write_graph_dot("graph.dot", graph);

	std::cout << "This is the end..." << std::endl;

	return EXIT_SUCCESS;
}
