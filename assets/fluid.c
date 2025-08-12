#include <stdlib.h>
#include <string.h>

#include "fluid.h"
#include "grid.h"
#include "config.h"

void fluid_destroy(FluidSystem *fluid) {
    if (!fluid) return;
   
	if (fluid->velocity_x) grid_destroy(fluid->velocity_x);
	if (fluid->velocity_prev_x) grid_destroy(fluid->velocity_prev_x);
	if (fluid->velocity_y) grid_destroy(fluid->velocity_y);
	if (fluid->velocity_prev_y) grid_destroy(fluid->velocity_prev_y);

    if (fluid->pressure) grid_destroy(fluid->pressure);
    if (fluid->density_prev) grid_destroy(fluid->density_prev);
    if (fluid->density) grid_destroy(fluid->density);
    if (fluid->divergence) grid_destroy(fluid->divergence);

    free(fluid);
}

FluidSystem* fluid_create(int width, int height) {
	FluidSystem *fluid = (FluidSystem*)malloc(sizeof(FluidSystem));
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
	fluid->divergence = grid_create(width, height);

	if (!fluid->velocity_x || !fluid->velocity_prev_x || !fluid->velocity_y || !fluid->velocity_prev_y || !fluid->density || !fluid->density_prev || !fluid->pressure || ! fluid->divergence) {
		fluid_destroy(fluid);
		return NULL;
	}

	fluid->delta_time = TIME_STEP;
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
	grid_clear(fluid->divergence);

}

void fluid_add_velocity(FluidSystem *fluid, int x, int y, float vx, float vy) {
	if (!fluid) return;

	grid_add_source(fluid->velocity_x, x, y, vx);
	grid_add_source(fluid->velocity_y, x, y, vy);
}

void fluid_add_density(FluidSystem *fluid, int x, int y, float amount) {
	if (!fluid) return;

	grid_add_source(fluid->density ,x, y, amount);
}

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

void diffuse(Grid2D *dest, Grid2D *src, float diff, float delta_time) {
	if (!dest || !src || diff <= 0.0f) return;

	grid_copy(dest, src);

	for (int iter = 0; iter < GAUSS_SEIDEL_ITERATION; iter ++) {
		for (int y = 0; y < src->height; y ++) {
			for (int x = 0; x < src->width; x++) {
				float lap = laplacian(dest, x, y);
				float new_val = (grid_get(src, x, y) + diff * delta_time * lap / (1.0f + 4.0f * diff * delta_time));

				grid_set(dest, x, y, new_val);
			}
		}
	}
}

void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div) {
	if (!u || !v || !pressure || !div) return;

	int width = u->width;
	int height = u->height;

	for (int y = 1; y < height -1; y++) {
		for (int x = 1; x < width - 1; x++) {
			float div_val = laplacian(u , x, y);
			grid_set(div, x, y, div_val);
		}
	}

	grid_clear(pressure);

	for (int iter = 0; iter < GAUSS_SEIDEL_ITERATION; iter ++) {
		for (int y = 1; y < height - 1; y++) {
			for (int x = 1; x < width - 1; x++) {
				float neighbours = laplacian(pressure, x, y);
				float new_pressure = (neighbours - grid_get(div, x, y)) / 4.0f;

				grid_set(pressure, x, y, new_pressure);
			}
		}
	}


	for (int y = 1; y < height - 1; y++) {
		for (int x = 1; x < width - 1; x++) {
			float grad_x = 0.5f * (grid_get(pressure, x + 1, y) - grid_get(pressure, x - 1, y));
			float grad_y = 0.5f * (grid_get(pressure, x, y + 1) - grid_get(pressure, x, y - 1));

			float new_u = grid_get(u, x, y) - grad_x;
			float new_v = grid_get(v, x, y) - grad_y;

			grid_set(u, x, y, new_u);
			grid_set(v, x, y, new_v);
		}
	}
}

void fluid_update(FluidSystem *fluid) {
	if (!fluid) return;

	float delta_time = fluid->delta_time;
	float visc = fluid->viscosity;
	float diff = fluid->diffusion;

	diffuse(fluid->velocity_prev_x, fluid->velocity_x, visc, delta_time);
	diffuse(fluid->velocity_prev_y, fluid->velocity_y, visc, delta_time);

	project(fluid->velocity_prev_x, fluid->velocity_prev_y, fluid->pressure, fluid->divergence);

	advect(fluid->velocity_x, fluid->velocity_prev_x, fluid->velocity_prev_x, fluid->velocity_prev_y, delta_time);
	advect(fluid->velocity_y, fluid->velocity_prev_y, fluid->velocity_prev_x, fluid->velocity_prev_y, delta_time);

	project(fluid->velocity_x, fluid->velocity_y, fluid->pressure, fluid->divergence);

	diffuse(fluid->density_prev, fluid->density, diff, delta_time);

	advect(fluid->density, fluid->density_prev, fluid->velocity_x, fluid->velocity_y, delta_time);
}

