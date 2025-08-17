
#ifndef FLUID_H
#define FLUID_H

#include <stdlib.h>
#include <string.h>

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
	Grid2D *divergence;
    
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
void fluid_destroy(FluidSystem *fluid);

/*
	fluid_create
	creates a FluidSystem
	@return FluidSystem* - returns the created fluidsystem
	@param width - the width of the FluidSystem to create 
	@param height - the height of the FluidSystem to create 
*/
FluidSystem* fluid_create(int width, int height);

/*
	fluid_reset
	resets a fluid to an empty state
	@param *fluid - the FluidSystem to reset
*/
void fluid_reset(FluidSystem *fluid);

/*
	fluid_add_velocity
	adds velocity to the fluid
	@param *fluid - the FluidSystem to update the velocity in
	@param x - the x position to update
	@param y - the y position to update
	@param vx - the velocity to add to the x axis
	@param vy - the velocity to add to the y axis
*/
void fluid_add_velocity(FluidSystem *fluid, int x, int y, float vx, float vy);

/*
	fluid_add_density
	adds density to a fluid 
	@param *fluid - the fluid to add the density to 
	@param x - the position on the x axis
	@param y - the position on the y axis
	@param amount - how much to add 
*/ 
void fluid_add_density(FluidSystem *fluid, int x, int y, float amount);

/*
	advect
	calculates where a fluid at (x, y) came from delta_time seconds ago 
	@param *dest - the destenation to set the new changes to 
	@param *src - the source to look at 
	@param *u - the grid with the velocity in the x axis
	@param *v - thegrid with the velocity in the y axis 
	@param delta_time - how long you have to look back
	@param b - the type of boundary to apply 
*/
void advect(Grid2D *dest, Grid2D *src, Grid2D *u, Grid2D *v, float delta_time);

/*
	diffuse
	diffuses a scalar grid using the gauss-seidel method
	@param *dest - the destenation to write to 
	@param *src - the source to start with
	@param diff - the diffusion rate 
	@param delta_time - the delta time 
*/
void diffuse(Grid2D *dest, Grid2D *src, float diff, float delta_time);

/*
	diffuse_velocity
	diffuses a velocity grid
	@param *dest_u - the destenation for the x velocity
	@param *src_u - the source for the x velocity
	@param *dest_v - the destenation for the y velocity
	@param *src_v - the source for the y velocity
	@param visc - the viscosity of the fluid
	@param delta_time - the delta time 
*/
void diffuse_velocity(Grid2D *dest_u, Grid2D *src_u, Grid2D *dest_v, Grid2D *src_v, float visc, float delta_time);

/*
	project 
	makes sure no new liquid appear or disappear using Poisson equation and the gauss-seidel method
	@param *u - the grid with velocity in the x axis
	@param *v - the grid with velocity in the y axis
	@param *pressure - the grid contaning the pressure
	@param *div - the divergence grid 
*/
void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div);

/*
	apply_gravity
	applies gravity to a fluidsystem 
	@param *fluid - the fluidsystem to apply gravity to
	@param delta_time - how much delta time has passed
*/
void apply_gravity(FluidSystem *fluid, float delta_time);

/*
	set_boundary
	sets the boundaries on a fluid 
*/
void set_bnd(FluidSystem *fluid);

/*
	fluid_update
	updates the fluid 
	@param *fluid - the fluid to update 
*/
void fluid_update(FluidSystem *fluid);

#endif
