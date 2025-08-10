#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Grid dimensions
#define GRID_WIDTH 80
#define GRID_HEIGHT 24

// Physics parameters
#define TIME_STEP 0.016f    // ~60 FPS
#define VISCOSITY 0.1f
#define DIFFUSION 0.0001f
#define GAUSS_SEIDEL_ITERATION 20 

// Rendering settings
#define DENSITY_SCALE 10.0f
#define VELOCITY_SCALE 5.0f

// Color pairs (for ncurses)
#define COLOR_FLUID 1
#define COLOR_VELOCITY 2
#define COLOR_OBSTACLE 3

#endif

