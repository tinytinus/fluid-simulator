#include "renderer.h"

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"

void renderer_destroy(Renderer *renderer) {
  if (renderer) {
    free(renderer);
  }
}

void renderer_init_colors(Renderer *renderer) {
  if (!renderer || !renderer->use_colors)
    return;

  init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(2, COLOR_BLUE, COLOR_BLACK);
  init_pair(3, COLOR_CYAN, COLOR_BLACK);
  init_pair(4, COLOR_WHITE, COLOR_BLACK);
  init_pair(9, COLOR_YELLOW, COLOR_BLACK);
}

Renderer *renderer_create(int width, int height) {
  Renderer *renderer = (Renderer *)malloc(sizeof(Renderer));
  if (!renderer)
    return NULL;

  renderer->width = width;
  renderer->height = height;
  renderer->use_colors = has_colors();
  renderer->density_treshold = 0.01f;

  if (renderer->use_colors) {
    renderer_init_colors(renderer);
  }

  return renderer;
}

void renderer_clear(Renderer *renderer) {
  if (!renderer)
    return;
  clear();
}

char density_to_char(float density) {
  if (density < 0.1f)
    return ' ';
  else if (density < 0.3f)
    return '.';
  else if (density < 0.6f)
    return ':';
  else if (density < 0.9f)
    return '#';
  else
    return '@';
}

int density_to_color(float density) {
  if (density < 0.5f)
    return 1;
  if (density < 2.0f)
    return 2;
  if (density < 5.0f)
    return 3;
  else
    return 4;
}

void draw_status(Renderer *renderer, FluidSystem *fluid, InputState *input) {
  if (!renderer)
    return;

  float total_density = 0;
  float max_density = 0;
  int fluid_cells = 0;

  for (int x = 0; x < fluid->width; x++) {
    for (int y = 0; y < fluid->height; y++) {
      float d = grid_get(fluid->density, x, y);
      total_density += d;
      if (d > 0.01f)
        fluid_cells++;
      if (d > max_density)
        max_density = d;
    }
  }

  char status[256];
  snprintf(status, sizeof(status),
           "Total: %.2f | Max: %.2f | Cells: %d | %s | 'S'=Step Mode:%s | "
           "'N'=Next Step | 'P'=Pause Press 'q' to quit",
           total_density, max_density, fluid_cells,
           input->paused ? "PAUSED" : "RUNNING",
           input->step_mode ? "ON" : "OFF");

  // density debugging
  float density_total = 0, density_prev_total = 0;
  float vel_x_total = 0, vel_y_total = 0;

  for (int x = 0; x < fluid->width; x++) {
    for (int y = 0; y < fluid->height; y++) {
      density_total += grid_get(fluid->density, x, y);
      density_prev_total += grid_get(fluid->density_prev, x, y);
      vel_x_total += fabs(grid_get(fluid->velocity_x, x, y));
      vel_y_total += fabs(grid_get(fluid->velocity_y, x, y));
    }
  }

  if (renderer->use_colors) {
    attron(COLOR_PAIR(9));

    mvprintw(0, 0, "%s", status);

    mvprintw(2, 0, "Density:      %8.3f", density_total);
    mvprintw(3, 0, "Density Prev: %8.3f", density_prev_total);
    mvprintw(4, 0, "Vel X Total:  %8.3f", vel_x_total);
    mvprintw(5, 0, "Vel Y Total:  %8.3f", vel_y_total);

    attroff(COLOR_PAIR(9));
  } else {
    mvprintw(0, 0, "%s", status);

    mvprintw(2, 0, "Density:      %8.3f", density_total);
    mvprintw(3, 0, "Density Prev: %8.3f", density_prev_total);
    mvprintw(4, 0, "Vel X Total:  %8.3f", vel_x_total);
    mvprintw(5, 0, "Vel Y Total:  %8.3f", vel_y_total);
  }
}

void renderer_draw_fluid(Renderer *renderer, FluidSystem *fluid) {
  if (!renderer || !fluid)
    return;

  for (int y = 0; y < renderer->height && y < fluid->height; y++) {
    for (int x = 0; x < renderer->width && x < fluid->width; x++) {
      float density = grid_get(fluid->density, x, y);

      if (density < renderer->density_treshold) {
        mvaddch(y, x, ' ');
        continue;
      }

      char ch = density_to_char(density);
      if (renderer->use_colors) {
        int color_pair = density_to_color(density);
        attron(COLOR_PAIR(color_pair));
        mvaddch(y, x, ch);
        attroff(COLOR_PAIR(color_pair));
      } else {
        mvaddch(y, x, ch);
      }
    }
  }
}

void renderer_present(Renderer *renderer) {
  if (!renderer)
    return;
  refresh();
}
