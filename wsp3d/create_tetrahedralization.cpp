#include "common.h"
#include "create_tetrahedralization.h"

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