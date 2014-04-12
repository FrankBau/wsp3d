
// developed from http://doc.cgal.org/latest/Triangulation_3/Triangulation_3_2regular_3_8cpp-example.html

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Regular_triangulation_euclidean_traits_3.h>

#include <cassert>
#include <vector>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Regular_triangulation_euclidean_traits_3<K>  Traits;
typedef Traits::RT                                          Weight;
typedef Traits::Bare_point                                  Point;
typedef Traits::Weighted_point                              Weighted_point;
typedef CGAL::Regular_triangulation_3<Traits>               Rt;
typedef Rt::Vertex_iterator                                 Vertex_iterator;
typedef Rt::Vertex_handle                                   Vertex_handle;

int main()
{
	// const int n = 101;	
	const int n = 2;

	std::cout << "preparing vertex vector ..." << std::endl;
	// generate points on a 3D grid
	std::vector<Weighted_point> P;
	int number_of_points = 0;
	for (int z = 0; z<n; z++)
		for (int y = 0; y<n; y++)
			for (int x = 0; x<n; x++) {
				Point p(x, y, z);
				Weight w = 1; // let's say this is the weight.
				P.push_back(Weighted_point(p, w));
				++number_of_points;
			}

	std::cout << "insert vertices in triangulation..." << std::endl;
	Rt T;
	// insert all points in a row (this is faster than one insert() at a time).
	T.insert(P.begin(), P.end());
	assert(T.is_valid());
	assert(T.dimension() == 3);
	std::cout << "Number of vertices : " << T.number_of_vertices() << std::endl;
	std::cout << "Number of finite cells : " << T.number_of_finite_cells() << std::endl;
	// edges and faces can be queried, but do not make sense

#if 1 // DUMP_CELLS
	for (auto cell = T.finite_cells_begin(); cell != T.finite_cells_end(); cell++)
	{
		for (int i = 0; i < 4; i++)
			std::cout << "( " << cell->vertex(i)->point() << " ), ";
		std::cout << std::endl;
	}
#endif

	std::cout << "This is the end..." << std::endl;
	return 0;
}

