
#ifndef FLUID_H
#define FLUID_H

#include "grid.h"

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
FluidSystem* fluid_create(int width, int height, float dt, float viscosity, float diffusion) {
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

	if (!fluid->velocity_x || !velocity_prev_x || !velocity_y || !velocity_prev_y || !density || !density_prev || !pressure) {
		fluid_destroy(fluid);
		return NULL;
	}

	fluid->dt = dt;
	fluid->viscosity = viscosity;
	fluid->diffusion = diffusion;
}

void fluid_destroy(FluidSystem *fluid) {

}

void fluid_reset(FluidSystem *fluid);

// Main simulation step
void fluid_update(FluidSystem *fluid);

// Force/source application
void fluid_add_velocity(FluidSystem *fluid, int x, int y, float vx, float vy);
void fluid_add_density(FluidSystem *fluid, int x, int y, float amount);

// Core solver functions (internal)
void advect(Grid2D *dest, Grid2D *src, Grid2D *u, Grid2D *v, float dt);
void diffuse(Grid2D *dest, Grid2D *src, float diff, float dt);
void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div);

#endif
