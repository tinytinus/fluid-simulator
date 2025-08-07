
#ifndef FLUID_H
#define FLUID_H

#include "grid.h"
#include "config.h"

typedef struct {
    // Velocity fields
   	Grid2D *velocity_x, *velocity_prev_x;
	Grid2D *velocity_y, *velocity_prev_y;


    // Scalar fields
    Grid2D *density, *density_prev;
    Grid2D *pressure;
    
    // Simulation parameters
    float dt, viscosity, diffusion;
    int width, height;
} FluidSystem;

// System management

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

	fluid->dt = TIME_STEP;
	fluid->viscosity = VISCOSITY;
	fluid->diffusion = DIFFUSION;

	return fluid;
}

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

// Force/source application
void fluid_add_velocity(FluidSystem *fluid, int x, int y, float vx, float vy) {
	
}

void fluid_add_density(FluidSystem *fluid, int x, int y, float amount);

// Core solver functions (internal)
void advect(Grid2D *dest, Grid2D *src, Grid2D *u, Grid2D *v, float dt);
void diffuse(Grid2D *dest, Grid2D *src, float diff, float dt);
void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div);

#endif
