#include "write_vrml.h"

#include <CGAL/IO/VRML_2_ostream.h>

#include <iostream>
#include <fstream>

bool write_vrml(Triangulation& triangulation, std::string filename)
{
	std::ofstream file(filename, std::ios::trunc);
	if (!file.is_open())
	{
		std::cerr << "failed to open file " << filename << std::endl;
		return false;
	}

	CGAL::VRML_2_ostream vrml(file);

	for (auto cell = triangulation.finite_cells_begin(); cell != triangulation.finite_cells_end(); ++cell)
	{
		vrml << triangulation.tetrahedron(cell);
	}

	return true;
}