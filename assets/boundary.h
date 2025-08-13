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
    safe_grid_get
    Safe grid access with boundary handling
    @param grid - grid to access
    @param x - x coordinate (can be out of bounds)
    @param y - y coordinate (can be out of bounds)  
    @return value at (x,y) with boundary conditions applied
*/
float safe_grid_get(Grid2D *grid, int x, int y);

#endif
