#include "common.h"

#include "create_tetrahedralization.h"
#include "dump_triangulation.h"
#include "create_steiner_points.h"

#include "read_off.h"
#include "write_off.h"
#include "write_vrml.h"
#include "write_vtk.h"
#include "write_graph_dot.h"

#include <iostream>
#include <fstream>

int main()
{
	Triangulation triangulation;

	create_cubes(triangulation, 2, 2, 1 );
	// read_off(triangulation, "C:/Carleton/Meshes/holmes_off/geometry/octahedron.off"); 
	// read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/cube.off");
	// read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/ellipsoid.off");
	set_cell_and_vertex_ids(triangulation);
	set_random_weights(triangulation);
	propagate_weights(triangulation);

	std::cout << "Number of finite vertices : " << triangulation.number_of_vertices() << std::endl;
	std::cout << "Number of finite edges    : " << triangulation.number_of_finite_edges() << std::endl;
	std::cout << "Number of finite facets   : " << triangulation.number_of_finite_facets() << std::endl;
	std::cout << "Number of finite cells    : " << triangulation.number_of_finite_cells() << std::endl;

	write_vtk(triangulation,"tetrahedralization.vtk");

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
	
	write_shortest_path_vtk(graph, predecessors, distances, "shortest_path.vtk");

	write_graph_dot("graph.dot", graph);

	std::cout << "This is the end..." << std::endl;

	return EXIT_SUCCESS;
}
