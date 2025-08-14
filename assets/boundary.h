#ifndef BOUNDARY_H
#define BOUNDARY_H

#include "grid.h"

/*
    apply_scalar_boundaries
    Applies boundary conditions to scalar fields (density, pressure)
    @param grid - the grid to apply boundaries to
*/
void apply_scalar_boundaries(Grid2D *grid);

/*
    apply_velocity_boundaries  
    Applies boundary conditions to velocity fields
    @param u - x-velocity grid
    @param v - y-velocity grid
*/
void apply_velocity_boundaries(Grid2D *u, Grid2D *v);

/*
	set_boundary
	calls the correct function for the type of boundary
	@param b - the boundary type, 0 for scalar, 1 for velocity
	@param *scalar - the scalar grid to target
	@param *u - the x velocity grid to target
	@param *v - the y velocity grid to target
*/
void set_boundary(int b, Grid2D *scalar, Grid2D *u, Grid2D *v);

#endif
