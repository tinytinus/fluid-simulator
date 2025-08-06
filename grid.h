
#ifndef GRID_H
#define GRID_H

#include "math_utils.h"

/*
   Grid2D 
   a grid structure used to store a single float of data at each (x, y) position
   useful for pressure 
   data* - stores the floats with data
   width - the width of the grid
   height - the "height" of the grid (aka how many times the width)

*/
typedef struct {
    float *data;
    int width, height;
} Grid2D;

/*
   grid_create 
   creates a grid
   @return Grid2D* - returns the created grid
   @param width - the width of the grid
   @param height - the height of the grid
*/
Grid2D* grid_create(int width, int height) {
	Grid2D* grid = malloc(sizeof(Grid2D));
	grid->data = malloc(width * height * sizeof(float));
	grid->width = width;
	grid->height = height;

	memset(grid->data, 0, width * height * sizeof(float));
	return grid;	
}

/*
	grid_destroy
	destroys/frees the grid
	@param grid - which grid to free
*/
void grid_destroy(Grid2D *grid) {
	if (grid) {
		free(grid->data);
		free(grid);
	}
}

/*
   grid_clear 
   clears (zeros out) a grid
   @param grid - grid to clear
*/
void grid_clear(Grid2D *grid) {
	if (grid) {
		memset(grid->data, 0, grid->width * grid->height * sizeof(float));
	}
}

/*
   grid_copy
   copies a grid from source to destenation
   @param dest - the destenation to copy to
   @param src - the source to copy from	
*/
void grid_copy(Grid2D *dest, Grid2D *src) {
	if (dest->height == src->height && dest->width == src->width) {
		memcpy(dest->data, src->data, src->width * src->height * sizeof(float));
	}
}

/*
   grid_get
   gets the value from a position in a grid
   @return float - returns the value of the position
   @param *grid - the grid to get the value from
   @param x - position on the x axis
   @param y - position on the y axis
*/
float grid_get(Grid2D *grid, int x, int y) {
	if (grid && 0 <= x < grid->width && 0 <= y < grid->height) {
		return grid->data[y * grid->width + x];
	} else {
		return 0.0f;
	}
}

/*
   grid_set
   sets a position on the grid to a value
   @param *grid - the grid that gets targeted 
   @param x - the position on the x axis
   @param y - the position on the y axis 
   @param value - the value to set the position to
*/
void grid_set(Grid2D *grid, int x, int y, float value) {
	if (grid && 0 <= x < grid->width && 0 <= y < grid->height) {
		grid->data[y * grid->width + x] = value;
	}
}

/*
   grid_add_source
   adds a value to a position on the grid
   @param *grid - the grid to target 
   @param x - the position on the x axis
   @param y - the position on the y axis
   @param amount - the amount to increase the position by
*/
void grid_add_source(Grid2D *grid, int x, int y, float amount) {
	if (grid && 0 <= x < grid->width && 0 <= y < grid->height) {
		grid->data[y * grid->width + x] += amount;
	}
}

/*
   grid_interpolate
   takes a float value and gets the data from the integers around
   @return float - the interpolated value of the point
   @param *grid - the grid to target
   @param x - the float position of x
   @param y - the float position of y

*/
float grid_interpolate(Grid2D *grid, float x, float y) {
	 if (grid && 0 <= x < grid->width && 0 <= y < grid->height) {
		/*
			get the corners around the float
			x0 - left
			x1 - right
			y0 - up
			y1 - down
		*/
		int x0 = (int)x;
		int x1 = x0 + 1;
		int y0 = (int)y;
		int y1 = y0 + 1;

		/*
		   get the position in the box
		   fx - float x inside box
		   fy - float y inside box
		*/
		float fx = x - x0;
		float fy = y - y0;

		/*
		   get the values from the coners from the grid
		   tl - top left
		   tr - top right
		   bl - bottom left
		   br - bottom right
		*/
		float tl = grid_get(grid, x0, y0);
		float tr = grid_get(grid, x1, y0);
		float bl = grid_get(grid, x0, y1);
		float br = grid_get(grid, x1, y1);

		/*
		   interpolate and blend the values
		   simply put, take the percentage of how close it is and get the average based on those percentages 
		   @return - the interpolated value from the cube
		*/
		return bilinear_interpolate(tl, tr, bl, br, fx, fy);
	 }
}

#endif

