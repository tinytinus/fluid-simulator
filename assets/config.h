#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Grid dimensions
#define GRID_WIDTH 80
#define GRID_HEIGHT 24

// Physics parameters
#define TIME_STEP 0.016f    // 0.016f = ~60 FPS
#define VISCOSITY 0.01f
#define DIFFUSION 0.001f
#define GAUSS_SEIDEL_ITERATION 40 
#define GRAVITY 0.001f

// Rendering settings
#define DENSITY_SCALE 10.0f
#define VELOCITY_SCALE 5.0f

// input settings 
#define FLUID_ADD 5.0f

#endif

