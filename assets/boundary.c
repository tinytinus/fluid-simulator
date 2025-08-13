#include "boundary.h"

void apply_scalar_boundaries(Grid2D *grid) {
    if (!grid) return;
    
    int w = grid->width;
    int h = grid->height;
    
	for (int x = 0; x < w; x++) {
		grid_set(grid, x, 0, grid_get(grid, x, 1));
		grid_set(grid, x, h-1, grid_get(grid, x, h-2));
	}
	for (int y = 0; y < h; y++) {
		grid_set(grid, 0, y, grid_get(grid, 1, y));
		grid_set(grid, w-1, y, grid_get(grid, w-2, y));
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

float safe_grid_get(Grid2D *grid, int x, int y) {
    if (!grid) return 0.0f;
    
    int width = grid->width;
    int height = grid->height;
    
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid_get(grid, x, y);
    }
    
	if (x < 0) x = -x;
	if (x >= width ) x = 2 * (width - 1) - x;
	if (y < 0) y = -y;
	if (y >= height) y = 2 * (height - 1) - y;
	
	x = (x < 0) ? 0 : (x >= width) ? width - 1 : x;
	y = (y < 0) ? 0 : (y >= height) ? height - 1 : y;
	return grid_get(grid, x, y);
            
}
