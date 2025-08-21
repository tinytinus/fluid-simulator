#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "fluid.h"
#include "grid.h"

void fluid_destroy(FluidSystem *fluid) {
  if (!fluid)
    return;

  if (fluid->velocity_x)
    grid_destroy(fluid->velocity_x);
  if (fluid->velocity_prev_x)
    grid_destroy(fluid->velocity_prev_x);
  if (fluid->velocity_y)
    grid_destroy(fluid->velocity_y);
  if (fluid->velocity_prev_y)
    grid_destroy(fluid->velocity_prev_y);

  if (fluid->pressure)
    grid_destroy(fluid->pressure);
  if (fluid->density_prev)
    grid_destroy(fluid->density_prev);
  if (fluid->density)
    grid_destroy(fluid->density);
  if (fluid->divergence)
    grid_destroy(fluid->divergence);

  free(fluid);
}

FluidSystem *fluid_create(int width, int height) {
  FluidSystem *fluid = (FluidSystem *)malloc(sizeof(FluidSystem));
  if (!fluid)
    return NULL;

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

  if (!fluid->velocity_x || !fluid->velocity_prev_x || !fluid->velocity_y ||
      !fluid->velocity_prev_y || !fluid->density || !fluid->density_prev ||
      !fluid->pressure || !fluid->divergence) {
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
  if (!fluid)
    return;

  grid_add_source(fluid->velocity_x, x, y, vx);
  grid_add_source(fluid->velocity_y, x, y, vy);
}

void fluid_add_density(FluidSystem *fluid, int x, int y, float amount) {
  if (!fluid)
    return;

  grid_add_source(fluid->density, x, y, amount);
}

void advect(Grid2D *dest, Grid2D *src, Grid2D *u, Grid2D *v, float delta_time) {
  if (!dest || !src || !u || !v)
    return;

  grid_clear(dest);

  for (int x = 0; x < src->width; x++) {
    for (int y = 0; y < src->height; y++) {
      float velocity_x = grid_get(u, x, y);
      float velocity_y = grid_get(v, x, y);

      float prev_x = x - velocity_x * delta_time;
      float prev_y = y - velocity_y * delta_time;

      prev_x = fmaxf(0.0f, fminf(prev_x, src->width - 1.001f));
      prev_y = fmaxf(0.0f, fminf(prev_y, src->height - 1.001f));

      float value = grid_interpolate(src, prev_x, prev_y);

      grid_set(dest, x, y, value);
    }
  }
}

void diffuse(Grid2D *dest, Grid2D *src, float diff, float delta_time) {
  if (!dest || !src || diff <= 0.0f)
    return;

  grid_copy(dest, src);

  for (int iter = 0; iter < GAUSS_SEIDEL_ITERATION; iter++) {
    for (int y = 1; y < src->height - 1; y++) {
      for (int x = 1; x < src->width - 1; x++) {
        float new_val = (grid_get_physics(src, x, y, 0) +
                         diff * delta_time * laplacian_physics(dest, x, y, 0)) /
                        (1.0f + 4.0f * diff * delta_time);

        grid_set(dest, x, y, new_val);
      }
    }
  }
}

void diffuse_velocity(Grid2D *dest_u, Grid2D *src_u, Grid2D *dest_v,
                      Grid2D *src_v, float visc, float delta_time) {
  if (!dest_u || !src_u || !dest_v || !src_v || visc <= 0.0f)
    return;

  grid_copy(dest_u, src_u);
  grid_copy(dest_v, src_v);

  for (int iter = 0; iter < GAUSS_SEIDEL_ITERATION; iter++) {
    for (int y = 1; y < src_u->height - 1; y++) {
      for (int x = 1; x < src_u->width - 1; x++) {
        float new_u = (grid_get_physics(src_u, x, y, 1) +
                       visc * delta_time * laplacian_physics(dest_u, x, y, 1)) /
                      (1.0f + 4.0f * visc * delta_time);
        float new_v = (grid_get_physics(src_v, x, y, 2) +
                       visc * delta_time * laplacian_physics(dest_v, x, y, 2)) /
                      (1.0f + 4.0f * visc * delta_time);

        grid_set(dest_u, x, y, new_u);
        grid_set(dest_v, x, y, new_v);
      }
    }
  }
}

void project(Grid2D *u, Grid2D *v, Grid2D *pressure, Grid2D *div) {
  if (!u || !v || !pressure || !div)
    return;

  int width = u->width;
  int height = u->height;

  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      float div_val = 0.5f * ((grid_get_physics(u, x + 1, y, 1) -
                               grid_get_physics(u, x - 1, y, 1)) +
                              (grid_get_physics(v, x, y + 1, 2) -
                               grid_get_physics(v, x, y - 1, 2)));
      grid_set(div, x, y, div_val);
    }
  }

  grid_clear(pressure);

  for (int iter = 0; iter < GAUSS_SEIDEL_ITERATION; iter++) {
    for (int y = 1; y < height - 1; y++) {
      for (int x = 1; x < width - 1; x++) {
        float neighbors = grid_get_physics(pressure, x - 1, y, 0) +
                          grid_get_physics(pressure, x + 1, y, 0) +
                          grid_get_physics(pressure, x, y - 1, 0) +
                          grid_get_physics(pressure, x, y + 1, 0);
        float new_pressure = (neighbors - grid_get(div, x, y)) / 4.0f;
        grid_set(pressure, x, y, new_pressure);
      }
    }
  }

  for (int y = 1; y < height - 1; y++) {
    for (int x = 1; x < width - 1; x++) {
      float grad_x = 0.5f * (grid_get_physics(pressure, x + 1, y, 1) -
                             grid_get_physics(pressure, x - 1, y, 1));
      float grad_y = 0.5f * (grid_get_physics(pressure, x, y + 1, 2) -
                             grid_get_physics(pressure, x, y - 1, 2));

      float new_u = grid_get_physics(u, x, y, 1) - grad_x;
      float new_v = grid_get_physics(v, x, y, 2) - grad_y;

      grid_set(u, x, y, new_u);
      grid_set(v, x, y, new_v);
    }
  }
}

void apply_gravity(FluidSystem *fluid, float delta_time) {
  if (!fluid)
    return;

  for (int y = 0; y < fluid->height; y++) {
    for (int x = 0; x < fluid->width; x++) {
      if (grid_get_physics(fluid->density, x, y, 0) > 0.001f) {
        float current_vy = grid_get_physics(fluid->velocity_y, x, y, 2);
        grid_set(fluid->velocity_y, x, y, current_vy + GRAVITY * delta_time);
      }
    }
  }
}

void set_bnd(FluidSystem *fluid) {
  if (!fluid)
    return;

  int w = fluid->width;
  int h = fluid->height;

  for (int x = 0; x < w; x++) {
    grid_set(fluid->velocity_y, x, 0, -grid_get(fluid->velocity_y, x, 1));
    grid_set(fluid->velocity_y, x, h - 1,
             -grid_get(fluid->velocity_y, x, h - 2));

    grid_set(fluid->velocity_x, x, 0, grid_get(fluid->velocity_x, x, 1));
    grid_set(fluid->velocity_x, x, h - 1,
             grid_get(fluid->velocity_x, x, h - 2));

    grid_set(fluid->density, x, 0, grid_get(fluid->density, x, 1));
    grid_set(fluid->density, x, h - 1, grid_get(fluid->density, x, h - 2));
  }

  for (int y = 0; y < h; y++) {
    grid_set(fluid->velocity_x, 0, y, -grid_get(fluid->velocity_x, 1, y));
    grid_set(fluid->velocity_x, w - 1, y,
             -grid_get(fluid->velocity_x, w - 2, y));

    grid_set(fluid->velocity_y, 0, y, grid_get(fluid->velocity_y, 1, y));
    grid_set(fluid->velocity_y, w - 1, y,
             grid_get(fluid->velocity_y, w - 2, y));

    grid_set(fluid->density, 0, y, grid_get(fluid->density, 1, y));
    grid_set(fluid->density, w - 1, y, grid_get(fluid->density, w - 2, y));
  }

  grid_set(fluid->velocity_x, 0, 0,
           0.5f * (grid_get(fluid->velocity_x, 1, 0) +
                   grid_get(fluid->velocity_x, 0, 1)));
  grid_set(fluid->velocity_x, w - 1, 0,
           0.5f * (grid_get(fluid->velocity_x, w - 2, 0) +
                   grid_get(fluid->velocity_x, w - 1, 1)));
  grid_set(fluid->velocity_x, 0, h - 1,
           0.5f * (grid_get(fluid->velocity_x, 1, h - 1) +
                   grid_get(fluid->velocity_x, 0, h - 2)));
  grid_set(fluid->velocity_x, w - 1, h - 1,
           0.5f * (grid_get(fluid->velocity_x, w - 2, h - 1) +
                   grid_get(fluid->velocity_x, w - 1, h - 2)));

  grid_set(fluid->velocity_y, 0, 0,
           0.5f * (grid_get(fluid->velocity_y, 1, 0) +
                   grid_get(fluid->velocity_y, 0, 1)));
  grid_set(fluid->velocity_y, w - 1, 0,
           0.5f * (grid_get(fluid->velocity_y, w - 2, 0) +
                   grid_get(fluid->velocity_y, w - 1, 1)));
  grid_set(fluid->velocity_y, 0, h - 1,
           0.5f * (grid_get(fluid->velocity_y, 1, h - 1) +
                   grid_get(fluid->velocity_y, 0, h - 2)));
  grid_set(fluid->velocity_y, w - 1, h - 1,
           0.5f * (grid_get(fluid->velocity_y, w - 2, h - 1) +
                   grid_get(fluid->velocity_y, w - 1, h - 2)));

  grid_set(
      fluid->density, 0, 0,
      0.5f * (grid_get(fluid->density, 1, 0) + grid_get(fluid->density, 0, 1)));
  grid_set(fluid->density, w - 1, 0,
           0.5f * (grid_get(fluid->density, w - 2, 0) +
                   grid_get(fluid->density, w - 1, 1)));
  grid_set(fluid->density, 0, h - 1,
           0.5f * (grid_get(fluid->density, 1, h - 1) +
                   grid_get(fluid->density, 0, h - 2)));
  grid_set(fluid->density, w - 1, h - 1,
           0.5f * (grid_get(fluid->density, w - 2, h - 1) +
                   grid_get(fluid->density, w - 1, h - 2)));
}

void fluid_update(FluidSystem *fluid) {
  if (!fluid)
    return;

  float delta_time = fluid->delta_time;
  float visc = fluid->viscosity;
  float diff = fluid->diffusion;

  grid_copy(fluid->velocity_prev_x, fluid->velocity_x);
  grid_copy(fluid->velocity_prev_y, fluid->velocity_y);

  diffuse_velocity(fluid->velocity_x, fluid->velocity_prev_x, fluid->velocity_y,
                   fluid->velocity_prev_y, visc, delta_time);
  set_bnd(fluid);

  project(fluid->velocity_x, fluid->velocity_y, fluid->pressure,
          fluid->divergence);

  grid_copy(fluid->velocity_prev_x, fluid->velocity_x);
  grid_copy(fluid->velocity_prev_y, fluid->velocity_y);

  advect(fluid->velocity_x, fluid->velocity_prev_x, fluid->velocity_prev_x,
         fluid->velocity_prev_y, delta_time);
  advect(fluid->velocity_y, fluid->velocity_prev_y, fluid->velocity_prev_x,
         fluid->velocity_prev_y, delta_time);
  set_bnd(fluid);

  apply_gravity(fluid, delta_time);
  set_bnd(fluid);

  project(fluid->velocity_x, fluid->velocity_y, fluid->pressure,
          fluid->divergence);

  grid_copy(fluid->density_prev, fluid->density);
  diffuse(fluid->density, fluid->density_prev, diff, delta_time);
  set_bnd(fluid);

  grid_copy(fluid->density_prev, fluid->density);
  advect(fluid->density, fluid->density_prev, fluid->velocity_x,
         fluid->velocity_y, delta_time);
  set_bnd(fluid);
}
