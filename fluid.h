
#ifndef FLUID_H
#define FLUID_H

#include "grid.h"
#include "config.h"

/*
	FluidSystem
	the actual fluid itself 
	@param velocity_x - the velocity of a position in the x direction
	@param velocity_prev_x - the velocity of a position in the x direction a moment ago
	@param velocity_y - the velocity of a position in the y direction
	@param velocity_prev_y - the velocity of a position in the y direction a moment ago
	
*/
typedef struct {
    // Velocity fields
   	Grid2D *velocity_x, *velocity_prev_x;
	Grid2D *velocity_y, *velocity_prev_y;


    // Scalar fields
    Grid2D *density, *density_prev;
    Grid2D *pressure;
    
    // Simulation parameters
    float delta_time, viscosity, diffusion;
    int width, height;
} FluidSystem;

/*
	fluid_destroy
	destroy a fluid
	for the reason of ordering this needs to be infront of fluid_create
	@param *fluid - the fluid to destroy
*/
void fluid_destroy(FluidSystem *fluid) {
    if (!fluid) return;
   
	if (fluid->velocity_x) grid_destroy(fluid->velocity_x);
	if (fluid->velocity_prev_x) grid_destroy(fluid->velocity_prev_x);
	if (fluid->velocity_y) grid_destroy(fluid->velocity_y);
	if (fluid->velocity_prev_y) grid_destroy(fluid->velocity_prev_y);

    if (fluid->pressure) grid_destroy(fluid->pressure);
    if (fluid->density_prev) grid_destroy(fluid->density_prev);
    if (fluid->density) grid_destroy(fluid->density);
    
    free(fluid);
}

/*
	fluid_create
	creates a FluidSystem
	@return FluidSystem* - returns the created fluidsystem
	@param width - the width of the FluidSystem to create 
	@param height - the height of the FluidSystem to create 
*/
FluidSystem* fluid_create(int width, int height) {
	FluidSystem *fluid = malloc(sizeof(FluidSystem));
	if (!fluid) return NULL;

	fluid->width = width;
	fluid->height = height;

	fluid->velocity_x = grid_create(width, height);
	fluid->velocity_prev_x = grid_create(width, height);
	fluid->velocity_y = grid_create(width, height);
	fluid->velocity_prev_y = grid_create(width, height);
	fluid->density = grid_create(width, height);
	fluid->density_prev = grid_create(width, height);
	fluid->pressure = grid_create(width, height);

	if (!fluid->velocity_x || !fluid->velocity_prev_x || !fluid->velocity_y || !fluid->velocity_prev_y || !fluid->density || !fluid->density_prev || !fluid->pressure) {
		fluid_destroy(fluid);
		return NULL;
	}

	fluid->delta_time = TIME_STEP;
	fluid->viscosity = VISCOSITY;
	fluid->diffusion = DIFFUSION;

	return fluid;
}

/*
	fluid_reset
	resets a fluid to an empty state
	@param *fluid - the FluidSystem to reset
*/
void fluid_reset(FluidSystem *fluid) {
	grid_clear(fluid->velocity_x);
	grid_clear(fluid->velocity_prev_x);
	grid_clear(fluid->velocity_y);
	grid_clear(fluid->velocity_prev_y);

	grid_clear(fluid->pressure);
	grid_clear(fluid->density_prev);
	grid_clear(fluid->density);

}

// Main simulation step
void fluid_update(FluidSystem *fluid);

/*
	fluid_add_velocity
	adds velocity to the fluid
	@param *fluid - the FluidSystem to update the velocity in
	@param x - the x position to update
	@param y - the y position to update
	@param vx - the velocity to add to the x axis
	@param vy - the velocity to add to the y axis
*/
void fluid_add_velocity(FluidSystem *fluid, int x, int y, float vx, float vy) {
	if (!fluid) return;

	grid_add_source(fluid->velocity_x, x, y, vx);
	grid_add_source(fluid->velocity_y, x, y, vy);
}

/*
	fluid_add_density
	adds density to a fluid 
	@param *fluid - the fluid to add the density to 
	@param x - the position on the x axis
	@param y - the position on the y axis
	@param amount - how much to add 
*/ 
void fluid_add_density(FluidSystem *fluid, int x, int y, float amount) {
	if (!fluid) return;

	grid_add_source(fluid->density ,x, y, amount);
}

/*
	advect
	calculates where a fluid at (x, y) came from delta_time seconds ago 
	@param *dest - the destenation to set the new changes to 
	@param *src - the source to look at 
	@param *u - the grid with the velocity in the x axis
	@param *v - thegrid with the velocity in the y axis 
	@param delta_time - how long you have to look back
*/
void advect(Grid2D *dest, Grid2D *src, Grid2D *u, Grid2D *v, float delta_time) {
	if (!dest || !src || !u || !v) return; 

	for (int x = 0; x < src->width; x++) {
		for (int y = 0; y < src->height; y++) {
			float velocity_x = grid_get(u, x, y);
			float velocity_y = grid_get(v, x,y);

			float prev_x = x - velocity_x * delta_time;
			float prev_y = y - velocity_y * delta_time;

			float value = grid_interpolate(src, prev_x, prev_y);

			grid_set(dest, x, y, value);
		}
	}
}

/*
	diffuse
	diffuses a grid using the gauss-seidel method 
	(https://en.wikipedia.org/wiki/Gauss%E2%80%93Seidel_method)
*/
void diffuse(Grid2D *dest, Grid2D *src, float diff, float delta_time) {
	if (!dest || !src || diff <= 0.0f) return;

	grid_copy(dest, src);

	for (int iter = 0; iter < GAUSS_SIDEL_ITERATIONS; iter ++) {
		for (int y = 0; y < src->height; y ++) {
			for (int x = 0; x < src->width; x++) {

			}
		}
	}
}

void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div);

#endif
