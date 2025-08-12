#include <stdlib.h>
#include <string.h>

#include "grid.h"
#include "math_utils.h"

Grid2D* grid_create(int width, int height) {
	Grid2D* grid = (Grid2D*)malloc(sizeof(Grid2D));
	if (!grid) return NULL;

	grid->data = (float*)malloc(width * height * sizeof(float));
	if (!grid->data) {
		free(grid);
		return NULL;
	}

	grid->width = width;
	grid->height = height;

	memset(grid->data, 0, width * height * sizeof(float));
	return grid;	
}

void grid_destroy(Grid2D *grid) {
	if (grid) {
		free(grid->data);
		free(grid);
	}
}

void grid_clear(Grid2D *grid) {
	if (grid) {
		memset(grid->data, 0, grid->width * grid->height * sizeof(float));
	}
}

void grid_copy(Grid2D *dest, Grid2D *src) {
	if (dest->height == src->height && dest->width == src->width) {
		memcpy(dest->data, src->data, src->width * src->height * sizeof(float));
	}
}

float grid_get(Grid2D *grid, int x, int y) {
    if (!grid) return 0.0f;
    
    if (x < 0) x = -x;
    if (x >= grid->width) x = 2 * (grid->width - 1) - x;
    if (y < 0) y = -y;
    if (y >= grid->height) y = 2 * (grid->height - 1) - y;
    
    x = (x < 0) ? 0 : (x >= grid->width) ? grid->width - 1 : x;
    y = (y < 0) ? 0 : (y >= grid->height) ? grid->height - 1 : y;
    
    return grid->data[y * grid->width + x];
}

void grid_set(Grid2D *grid, int x, int y, float value) {
	if (grid &&  x >= 0 && x < grid->width &&  y >= 0 && y < grid->height) {
		grid->data[y * grid->width + x] = value;
	}
}

void grid_add_source(Grid2D *grid, int x, int y, float amount) {
	if (grid &&  x >= 0 && x < grid->width &&  y >= 0 && y < grid->height) {
		grid->data[y * grid->width + x] += amount;
	}
}

float grid_interpolate(Grid2D *grid, float x, float y) {
	 if (grid &&  x >= 0 && x < grid->width &&  y >= 0 && y < grid->height) {
		int x0 = (int)x;
		int x1 = x0 + 1;
		int y0 = (int)y;
		int y1 = y0 + 1;

		float fx = x - x0;
		float fy = y - y0;

		float tl = grid_get(grid, x0, y0);
		float tr = grid_get(grid, x1, y0);
		float bl = grid_get(grid, x0, y1);
		float br = grid_get(grid, x1, y1);

		return bilinear_interpolate(tl, tr, bl, br, fx, fy);
	}

	return 0.0f;
}

float laplacian(Grid2D *grid, int x, int y) {
	if (!grid) return 0.0f;
	
	float center = grid_get(grid, x, y);
    float left = grid_get(grid, x-1, y);
    float right = grid_get(grid, x+1, y);  
    float up = grid_get(grid, x, y-1);
    float down = grid_get(grid, x, y+1);

	return (left + right  + up + down) - 4.0f * center; 
}
