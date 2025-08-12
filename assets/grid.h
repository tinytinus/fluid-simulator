
#ifndef GRID_H
#define GRID_H

#include "math_utils.h"

/*
   Grid2D 
   a grid structure used to store a single float of data at each (x, y) position 
   *data - stores the floats with data
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
Grid2D* grid_create(int width, int height);

/*
	grid_destroy
	destroys/frees the grid
	@param grid - which grid to free
*/
void grid_destroy(Grid2D *grid);

/*
   grid_clear 
   clears (zeros out) a grid
   @param grid - grid to clear
*/
void grid_clear(Grid2D *grid);

/*
   grid_copy
   copies a grid from source to destenation
   @param dest - the destenation to copy to
   @param src - the source to copy from	
*/
void grid_copy(Grid2D *dest, Grid2D *src);

/*
   grid_get
   gets the value from a position in a grid
   @return float - returns the value of the position
   @param *grid - the grid to get the value from
   @param x - position on the x axis
   @param y - position on the y axis
*/
float grid_get(Grid2D *grid, int x, int y);

/*
   grid_set
   sets a position on the grid to a value
   @param *grid - the grid that gets targeted 
   @param x - the position on the x axis
   @param y - the position on the y axis 
   @param value - the value to set the position to
*/
void grid_set(Grid2D *grid, int x, int y, float value);

/*
   grid_add_source
   adds a value to a position on the grid
   @param *grid - the grid to target 
   @param x - the position on the x axis
   @param y - the position on the y axis
   @param amount - the amount to increase the position by
*/
void grid_add_source(Grid2D *grid, int x, int y, float amount);

/*
   grid_interpolate
   takes a float value and gets the data from the integers around
   @return float - the interpolated value of the point
   @param *grid - the grid to target
   @param x - the float position of x
   @param y - the float position of y
*/
float grid_interpolate(Grid2D *grid, float x, float y);

/*
	laplacian
	smooth out a cell depending on the values around it 
	@return float - the new smoothed value for the cell
	@param *grid - the grid to target
	@param x - the x position to smooth out 
	@param y - the y position to smooth out 
*/
float laplacian(Grid2D *grid, int x, int y);

#endif

