#include "common.h"

bool write_vtk(Triangulation& triangulation, std::string filename);

bool write_shortest_path_vtk(Graph& graph, std::vector<GraphNode_descriptor>& predecessors, std::vector<double>& distances, std::string filename);
