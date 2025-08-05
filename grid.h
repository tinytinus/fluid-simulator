
#ifndef GRID_H
#define GRID_H

typedef struct {
    float *data;
    int width, height;
} Grid2D;

// Grid management
Grid2D* grid_create(int width, int height) {
	Grid2D* grid = malloc(sizeof(Grid2D));
	grid->data = malloc(width * height * sizeof(float));
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
		memset(grid->data, 0, width * height * sizeof(float));
	}
}

void grid_copy(Grid2D *dest, Grid2D *src) {
	if (dest->height == src->height && dest->width == src->width) {
		memcpy(dest->data, src->data, src->width * src->height * sizeof(float));
	}
}

// Grid access (with bounds checking)
float grid_get(Grid2D *grid, int x, int y);
void grid_set(Grid2D *grid, int x, int y, float value);

// Utility functions
void grid_add_source(Grid2D *grid, int x, int y, float amount);
float grid_interpolate(Grid2D *grid, float x, float y);

#endif

