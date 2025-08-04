
#ifndef FLUID_H
#define FLUID_H

#include "grid.h"

typedef struct {
    // Velocity fields
    Grid2D *u, *v;           // Current velocity
    Grid2D *u_prev, *v_prev; // Previous velocity
    
    // Scalar fields
    Grid2D *density, *density_prev;
    Grid2D *pressure;
    
    // Simulation parameters
    float dt, viscosity, diffusion;
    int width, height;
} FluidSystem;

// System management
FluidSystem* fluid_create(int width, int height);
void fluid_destroy(FluidSystem *fluid);
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
