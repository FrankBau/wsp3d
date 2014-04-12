/*
  common include file
*/

#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_cell_base_3.h>
#include <CGAL/Triangulation_vertex_base_3.h>

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

	// now we can have members like:
	// Vertex_handle   vh;
	// Cell_handle     ch;

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
	My_cell_base() : _info(0) 
	{ 
		init();  
	}

	My_cell_base(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3)
		: _info(0), Cb(v0, v1, v2, v3) 
	{
		init();
	}

	My_cell_base(Vertex_handle v0, Vertex_handle v1, Vertex_handle v2, Vertex_handle v3, Cell_handle n0, Cell_handle n1, Cell_handle n2, Cell_handle n3)
		: _info(0), Cb(v0, v1, v2, v3, n0, n1, n2, n3) 
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

private:
	void init() 
	{ 
		_weight = HUGE_VALD;
		for (int i = 0; i < 4; ++i) 
			for (int j = 0; j < 4; ++j) 
				_w[i][j] = HUGE_VALD; 
	}
	
	int _info;
	double _weight;
	double _w[4][4]; // face (i==j) and edge (i!=j) weights
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

typedef CGAL::Triangulation_data_structure_3<My_vertex_base<Kernel>, My_cell_base<Kernel> >    Tds;

typedef CGAL::Delaunay_triangulation_3<Kernel, Tds>			Triangulation;

typedef Triangulation::Cell									Cell; 
typedef Triangulation::Facet								Facet;
typedef Triangulation::Edge									Edge;
typedef Triangulation::Point								Point;

typedef Triangulation::Facet_circulator						Facet_circulator;

typedef Triangulation::Vertex_handle                        Vertex_handle;
typedef Triangulation::Cell_handle							Cell_handle;

#endif
