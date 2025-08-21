
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "fluid.h"
#include "input.h"

InputState *input_create(void) {
  InputState *input = (InputState *)malloc(sizeof(InputState));
  if (!input)
    return NULL;

  input->paused = false;
  input->show_debug = false;
  input->step_mode = false;
  input->step_once = false;

  return input;
}

void input_destroy(InputState *input) {
  if (!input)
    return;

  free(input);
}

void screen_to_grid(int screen_x, int screen_y, int *grid_x, int *grid_y) {
  if (grid_x)
    *grid_x = screen_x;
  if (grid_y)
    *grid_y = screen_y;

  if (*grid_x < 0)
    *grid_x = 0;
  if (*grid_x >= GRID_WIDTH)
    *grid_x = GRID_WIDTH - 1;

  if (*grid_y < 0)
    *grid_y = 0;
  if (*grid_y >= GRID_HEIGHT)
    *grid_y = GRID_HEIGHT - 1;
}

void handle_keyboard_input(InputState *input, FluidSystem *fluid, int ch) {
  if (!input || !fluid)
    return;

  switch (ch) {
  case 'r':
  case 'R':
    fluid_reset(fluid);
    break;

  case 's':
  case 'S':
    input->step_mode = !input->step_mode;
    if (input->step_mode)
      input->paused = true;
    break;

  case 'n':
  case 'N':
    if (input->step_mode) {
      input->step_once = true;
    }
    break;

  case 'p':
  case 'P':
    input->paused = !input->paused;
    break;

  case 'd':
  case 'D':
    input->show_debug = !input->show_debug;
    break;

  case ' ':
    fluid_add_density(fluid, fluid->width / 2, fluid->height / 2, FLUID_ADD);
    break;
  }
}

bool input_handle(InputState *input, FluidSystem *fluid) {
  if (!input || !fluid)
    return false;

  int ch = getch();

  if (ch != ERR) {
    handle_keyboard_input(input, fluid, ch);

    if (ch == 'q' || ch == 'Q') {
      return false;
    }
  }

  return true;
}
