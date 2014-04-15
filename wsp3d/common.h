/*
  common include file
*/

#ifndef COMMON_H
#define COMMON_H

// some VC++2013 internally include stuff needed this
#define _SCL_SECURE_NO_WARNINGS

#include <cmath>
#include <cassert>

#include <vector>
#include <iostream>
#include <fstream>

// prevent min/max define in "Windows Kits\8.1\Include\shared\minwindef.h"
#define NOMINMAX

///////////////////////////// boost /////////////////////////////

// #define BOOST_ALL_NO_LIB
// #define BOOST_ERROR_CODE_HEADER_ONLY
// #define BOOST_CHRONO_HEADER_ONLY

#include <boost/config.hpp>
#include <boost/chrono.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/small_world_generator.hpp>

#include <boost/property_map/property_map.hpp>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random/uniform_real_distribution.hpp>

///////////////////////////// CGAL /////////////////////////////

#include <CGAL/Random.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/squared_distance_3.h>

#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_cell_base_3.h>
#include <CGAL/Triangulation_vertex_base_3.h>

///////////////////////////// boost Graph /////////////////////////////

struct GraphNode;
struct GraphEdge;

typedef boost::adjacency_list <
	boost::vecS,
	boost::vecS,
	boost::undirectedS,
	GraphNode,
	GraphEdge
	> Graph;

typedef boost::graph_traits<Graph>::vertex_descriptor GraphNode_descriptor;
typedef boost::graph_traits<Graph>::edge_descriptor   GraphEdge_descriptor;

///////////////////////////// CGAL Triangulation /////////////////////////////

template < class GT, class Vb = CGAL::Triangulation_vertex_base_3<GT> >
class My_vertex_base
	: public Vb
{
public:
	typedef typename Vb::Vertex_handle  Vertex_handle;
	typedef typename Vb::Cell_handle    Cell_handle;
	typedef typename Vb::Point          Point;
	template < class TDS2 >
	struct Rebind_TDS {
		typedef typename Vb::template Rebind_TDS<TDS2>::Other  Vb2;
		typedef My_vertex_base<GT, Vb2>                        Other;
	};

	// must implement all ctors of Vb:
	My_vertex_base() {}
	My_vertex_base(const Point& p)
		: Vb(p) {}
	My_vertex_base(const Point& p, Cell_handle c)
		: Vb(p, c) {}

	int& info() { return _info; };

private:
	int _info;
};

template < class GT, class Cb = CGAL::Triangulation_cell_base_3<GT> >
class My_cell_base
	: public Cb
{
public:
	typedef typename Cb::Vertex_handle  Vertex_handle;
	typedef typename Cb::Cell_handle    Cell_handle;
	typedef typename Cb::Point          Point;
	template < class TDS2 >
	struct Rebind_TDS {
		typedef typename Cb::template Rebind_TDS<TDS2>::Other  Cb2;
		typedef My_cell_base<GT, Cb2>                        Other;
	};

	// must implement all ctors of Cb:
	My_cell_base()
	{ 
		init();
	}

	My_cell_base(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3)
		: Cb(v0, v1, v2, v3) 
	{
		init();
	}

	My_cell_base(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3, Cell_handle n0, Cell_handle n1, Cell_handle n2, Cell_handle n3)
		: Cb(v0, v1, v2, v3, n0, n1, n2, n3) 
	{
		init();
	}

	// now we can have members like:
	// Vertex_handle   vh;
	// Cell_handle     ch;

	int& info()
	{ 
		return _info;
	};
	
	// weight of an edge
	double& weight(int i, int j)
	{
		assert(0 <= i && i < 4);
		assert(0 <= j && j < 4);
		assert(i != j);
		return _w[i][j];
	}

	// weight of an facet
	double& weight(int i)
	{
		assert(0 <= i && i < 4);
		return _w[i][i];
	}

	// weight of cell
	double& weight()
	{
		return _weight;
	}

	GraphNode_descriptor& node()
	{
		return _node;
	}

private:
	void init() 
	{

		for (int i = 0; i < 4; ++i) 
			for (int j = 0; j < 4; ++j) 
				_w[i][j] = std::numeric_limits<double>::max();
	}
	
	int _info = 0;
	double _weight = std::numeric_limits<double>::max();;
	double _w[4][4]; // face (i==j) and edge (i!=j) weights
	GraphNode_descriptor _node = 0;
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

typedef CGAL::Triangulation_data_structure_3<My_vertex_base<Kernel>, My_cell_base<Kernel> > Tds;

typedef CGAL::Delaunay_triangulation_3<Kernel, Tds> Triangulation;

// combinatorial types
typedef Triangulation::Cell                                 Cell;
typedef Triangulation::Facet                                Facet;
typedef Triangulation::Edge                                 Edge;
typedef Triangulation::Vertex                               Vertex;

// geometric equivalents
typedef Triangulation::Tetrahedron                          Tetrahedron;
typedef Triangulation::Triangle                             Triangle;
typedef Triangulation::Segment                              Segment;
typedef Triangulation::Point                                Point;

typedef Triangulation::Facet_circulator                     Facet_circulator;

typedef Triangulation::Vertex_handle                        Vertex_handle;
typedef Triangulation::Cell_handle                          Cell_handle;

///////////////////////////// boost Graph details /////////////////////////////

struct GraphNode
{
public:
	GraphNode()
	{
		// std::cout << "GraphNode()" << std::endl;
	}

	Cell_handle cell = 0;   // the cell it belongs to
	int i = -1;             // index i (face related node) 
	int j = -1;             // rsp. i,j (edge related node)
	Point point;
};

struct GraphEdge
{
	// we could have done this with internal properties too
	double weight;
};

#endif
