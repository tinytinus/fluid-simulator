#include <assert.h>
#include <stdio.h>

#include "assets/fluid.h"

#define TEST(name) printf("Testing %s... ", #name);
#define PASS() printf("PASS\n");
#define FAIL(msg)
printf("FAIL: %s\n", msg);
exit(1);

void test_math_functions() {
  TEST(vec2_create);
  Vec2 v = vec2_create(3.0f, 4.0f);
  assert(v.x == 3.0f && v.y == 4.0f);
  PASS();

  TEST(vec2_magnitude);
  float mag = vec2_height(v);
  assert(mag == 5.0f); // 3-4-5 triangle
  PASS();

  TEST(lerp);
  float result = lerp(0.0f, 10.0f, 0.5f);
  assert(result == 5.0f);
  PASS();

  TEST(clamp);
  assert(clamp(15.0f, 0.0f, 10.0f) == 10.0f);
  assert(clamp(-5.0f, 0.0f, 10.0f) == 0.0f);
  assert(clamp(5.0f, 0.0f, 10.0f) == 5.0f);
  PASS();
}

void test_grid_functions() {
  Grid2D *grid = grid_create(5, 5);

  TEST(grid_set_get);
  grid_set(grid, 2, 2, 42.0f);
  assert(grid_get(grid, 2, 2) == 42.0f);
  PASS();

  TEST(grid_bounds_checking);
  assert(grid_get(grid, -1, 0) == 0.0f);
  assert(grid_get(grid, 10, 0) == 0.0f);
  PASS();

  TEST(grid_clear);
  grid_clear(grid);
  assert(grid_get(grid, 2, 2) == 0.0f);
  PASS();

  TEST(grid_add_source);
  grid_add_source(grid, 1, 1, 5.0f);
  grid_add_source(grid, 1, 1, 3.0f);
  assert(grid_get(grid, 1, 1) == 8.0f);
  PASS();

  grid_destroy(grid);
}

void test_fluid_functions() {
  FluidSystem *fluid = fluid_create(10, 10);

  TEST(fluid_add_density);
  fluid_add_density(fluid, 5, 5, 100.0f);
  assert(grid_get(fluid->density, 5, 5) == 100.0f);
  PASS();

  TEST(fluid_add_velocity);
  fluid_add_velocity(fluid, 3, 3, 10.0f, 20.0f);
  assert(grid_get(fluid->velocity_x, 3, 3) == 10.0f);
  assert(grid_get(fluid->velocity_y, 3, 3) == 20.0f);
  PASS();

  TEST(fluid_reset);
  fluid_reset(fluid);
  assert(grid_get(fluid->density, 5, 5) == 0.0f);
  assert(grid_get(fluid->velocity_x, 3, 3) == 0.0f);
  PASS();

  fluid_destroy(fluid);
}

int main() {
  printf("Running batch tests...\n");
  test_math_functions();
  test_grid_functions();
  test_fluid_functions();
  printf("All batch tests passed!\n");
  return 0;
}
