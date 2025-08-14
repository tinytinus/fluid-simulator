#include "boundary.h"
#include "grid.h"

void apply_scalar_boundaries(Grid2D *grid) {
    if (!grid) return;
    
    int width = grid->width;
    int height = grid->height;
    
	for (int x = 0; x < width; x++) {
		grid_add_source(grid, x, 1, grid_get(grid, x, 0));
		grid_add_source(grid, x, height - 1, grid_get(grid, x, height - 2));
	}
	for (int y = 0; y < height; y++) {
		grid_add_source(grid, 1, y, grid_get(grid, 0, y));
		grid_add_source(grid, width - 1, y, grid_get(grid, width - 2, y));
		// the fluid needs to be removed from where it came from so that it doesnt double
	}
            
}

void apply_velocity_boundaries(Grid2D *u, Grid2D *v) {
    if (!u || !v) return;
    
    int width = u->width;
    int height = u->height;
    
	for (int x = 0; x < width; x++) {
		grid_set(u, x, 0, grid_get(u, x, 1));        
		grid_set(u, x, height - 1, grid_get(u, x, height - 2));
		grid_set(v, x, 0, -grid_get(v, x, 1));       
		grid_set(v, x, height - 1, -grid_get(v, x, height - 2));
	}
	for (int y = 0; y < height; y++) {
		grid_set(u, 0, y, -grid_get(u, 1, y));      
		grid_set(u, width - 1, y, -grid_get(u, width - 2, y));
		grid_set(v, 0, y, grid_get(v, 1, y));
		grid_set(v, width - 1, y, grid_get(v, width - 2, y));
	}
}

void set_boundary(int b, Grid2D *scalar, Grid2D *u, Grid2D *v ) {
	if (!scalar || !u || !v) return;

	if (b == 0) {
		apply_scalar_boundaries(scalar);
	} else if (b == 1) {
		apply_velocity_boundaries(u, v);
	}
}
