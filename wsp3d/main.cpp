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

	boost::filesystem::path input_pathname = "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/elephant.off";

	// create_cubes(triangulation, 2, 2, 1 );
	read_off( triangulation, input_pathname.string() );

	// read_off(triangulation, "C:/Carleton/Meshes/holmes_off/geometry/octahedron.off"); 
	// read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/cube.off");
	// read_off(triangulation, "C:/Carleton/CGAL-4.4/demo/Polyhedron/data/ellipsoid.off");

#if 0
	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		for (int i = 0; i < 4; ++i)
		{
			Point p = cell->vertex(i)->point();
			assert(-10.0 < p.x() && p.x() < +10.0);
			assert(-10.0 < p.y() && p.y() < +10.0);
			assert(-10.0 < p.z() && p.z() < +10.0);
		}
	}
#endif

	set_cell_and_vertex_ids(triangulation);
	set_random_weights(triangulation);
	propagate_weights(triangulation);

	std::cout << "Number of finite vertices : " << triangulation.number_of_vertices() << std::endl;
	std::cout << "Number of finite edges    : " << triangulation.number_of_finite_edges() << std::endl;
	std::cout << "Number of finite facets   : " << triangulation.number_of_finite_facets() << std::endl;
	std::cout << "Number of finite cells    : " << triangulation.number_of_finite_cells() << std::endl;

	std::string filename = input_pathname.filename().stem().string() + "_tet.vtk";
	write_vtk( triangulation, filename );

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
	
	filename = input_pathname.filename().stem().string() + "_wsp.vtk";
	write_shortest_path_vtk( graph, predecessors, distances, filename );

	// write_graph_dot("graph.dot", graph);

	std::cout << "This is the end..." << std::endl;

	return EXIT_SUCCESS;
}
