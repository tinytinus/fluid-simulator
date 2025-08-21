
#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fluid.h"
#include "grid.h"
#include "input.h"

/*
        Renderer
        stores data about the rendering
*/
typedef struct {
  int width, height;
  bool use_colors;
  float density_treshold;
} Renderer;

/*
        renderer_destroy
        destroys a renderer
        @param *renderer - the renderer to destroy
*/
void renderer_destroy(Renderer *renderer);

/*
        renderer_init_colors
        initializes the colors
        @param *renderer - used to check if the renderer still exist and uses
   colors
*/
void renderer_init_colors(Renderer *renderer);

/*
        renderer_create
        creates a renderer
        @return Renderer* - returns the created renderer
        @param width - the width of the renderer to create
        @param height - the height of the renderer to create
*/
Renderer *renderer_create(int width, int height);

/*
        renderer_clear
        clears the screen
        @param renderer - the renderer to check if it exsists
*/
void renderer_clear(Renderer *renderer);

/*
        density_to_char
        takes the density and returns a coresponding char
        @return char - the char returned
        @param density - the density to get the coresponding char from
*/
char density_to_char(float density);

/*
        density_to_color
        takes the density and converts it into a color
        @return int - returns the int of the color
        @param density - the density to get the color from
*/
int density_to_color(float density);

/*
        draw_status
        draws the status / debug information
        @param *renderer - the renderer to draw the information on
        @param *status - what actually to draw
*/
void draw_status(Renderer *renderer, FluidSystem *fluid, InputState *input);

/*
        renderer_draw_fluid
        draws the fluid to the screen
        @param *renderer - the renderer to get drawing data from
        @param *fluid - the FluidSystem to draw
*/
void renderer_draw_fluid(Renderer *renderer, FluidSystem *fluid);

/*
        renderer_present
        presents the rendered renderer
        @param renderer - to ensure it exsists
*/
void renderer_present(Renderer *renderer);

#endif
