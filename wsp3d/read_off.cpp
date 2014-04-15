#include "read_off.h"

#include <fstream>

/*
	read vertex coordinates from .off file but ignores facets
*/
bool read_off(Triangulation& triangulation, std::string filename)
{
	std::ifstream file;
	// file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	file.open(filename);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename << std::endl;
		exit(EXIT_FAILURE);
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

	std::vector<Point> pointVector;
	for (int i = 0; i < v; ++i)
	{
		double x, y, z;

		if (!(file >> x))
		{
			std::cerr << "failed to read x for point " << i << ", exit";
			exit(EXIT_FAILURE);
		}

		if (!(file >> y))
		{
			std::cerr << "failed to read y for point " << i << ", exit";
			exit(EXIT_FAILURE);
		}

		if (!(file >> z))
		{
			std::cerr << "failed to read z for point " << i << ", exit";
			exit(EXIT_FAILURE);
		}

		Point point(x, y, z);

		pointVector.push_back(point);
	}

	triangulation.insert(pointVector.begin(), pointVector.end());
	assert(triangulation.is_valid());
	assert(triangulation.dimension() == 3);

	file.close();

	return true;
}
