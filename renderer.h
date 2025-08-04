
#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include "fluid.h"

typedef struct {
    int width, height;
    bool use_colors;
    float density_threshold;
} Renderer;

// Renderer management
Renderer* renderer_create(int width, int height);
void renderer_destroy(Renderer *renderer);
void renderer_init_colors(Renderer *renderer);

// Main rendering functions
void renderer_clear(Renderer *renderer);
void renderer_draw_fluid(Renderer *renderer, FluidSystem *fluid);
void renderer_draw_velocity(Renderer *renderer, FluidSystem *fluid);
void renderer_present(Renderer *renderer);

// Utility functions
char density_to_char(float density);
int density_to_color(float density);
void draw_status(Renderer *renderer, const char *status);

#endif
