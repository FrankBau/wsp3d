#include "read_off.h"

#include <fstream>

/*
	read vertex coordinates from .off file but ignores facets
*/
bool read_off(Triangulation& triangulation, std::string filename)
{
	std::ifstream file;
	file.open(filename);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename << std::endl;
		return false;
	}

	std::cout << "reading file " << filename << std::endl;

	std::string line;
	
	std::getline(file, line);
	if(line != "OFF")
	{
		std::cerr << "missing OFF header";
		return false;
	}

	std::getline(file, line);
	std::stringstream ss(line);
	int v, f, e;
	ss >> v;
	ss >> f;
	ss >> e;

	std::vector< std::pair<Point, int> > pointVector;
	for (int i = 0; i < v; ++i)
	{
		std::getline(file, line);
		std::stringstream ss(line);
		double x, y, z;
		ss >> x;
		ss >> y;
		ss >> z;
		Point point(x, y, z);
		pointVector.push_back(std::make_pair(point, i));
	}

	triangulation.insert(pointVector.begin(), pointVector.end());
	assert(triangulation.is_valid());
	assert(triangulation.dimension() == 3);

	file.close();

	return true;
}
