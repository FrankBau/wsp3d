#include "common.h"

/*
	set cell and vertex id's
*/
void set_cell_and_vertex_ids(Triangulation& triangulation);

/*
	sets numbering of cells and vertices, random weights
*/
void set_random_weights(Triangulation& triangulation);

/*
	propagates weights from cells to facets and edges 
	(propagated weights are accesible through cell)
*/
void propagate_weights(Triangulation& triangulation);

/*
	creates an n x m x k array of tetrahedralized cubes
*/
void create_cubes(Triangulation& triangulation, int n, int m, int k);
