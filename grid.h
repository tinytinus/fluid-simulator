
#ifndef GRID_H
#define GRID_H

typedef struct {
    float *data;
    int width, height;
} Grid2D;

// Grid management
Grid2D* grid_create(int width, int height);
void grid_destroy(Grid2D *grid);
void grid_clear(Grid2D *grid);
void grid_copy(Grid2D *dest, Grid2D *src);

// Grid access (with bounds checking)
float grid_get(Grid2D *grid, int x, int y);
void grid_set(Grid2D *grid, int x, int y, float value);

// Utility functions
void grid_add_source(Grid2D *grid, int x, int y, float amount);
float grid_interpolate(Grid2D *grid, float x, float y);

#endif

