
#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "fluid.h"
#include "grid.h"

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
void renderer_destroy(Renderer *renderer) {
	if (renderer) {
		free(renderer);
	}
}

/*
	renderer_init_colors
	initializes the colors
	@param *renderer - used to check if the renderer still exist and uses colors
*/
void renderer_init_colors(Renderer *renderer) {
	if (!renderer || !renderer->use_colors) return;

	init_pair(1, COLOR_BLUE, COLOR_BLACK);   
    init_pair(2, COLOR_CYAN, COLOR_BLACK);     
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);
}

/*
	renderer_create
	creates a renderer
	@return Renderer* - returns the created renderer 
	@param width - the width of the renderer to create 
	@param height - the height of the renderer to create 
*/ 
Renderer* renderer_create(int width, int height) {
	Renderer *renderer = (Renderer *)malloc(sizeof(Renderer));
	if (!renderer) return NULL;

	renderer->width = width;
	renderer->height = height;
	renderer->use_colors = has_colors();
	renderer->density_treshold = 0.01f;

	if (renderer->use_colors) {
		renderer_init_colors(renderer);
	}
	
	return renderer;
}

/*
	renderer_clear
	clears the screen
	@param renderer - the renderer to check if it exsists
*/
void renderer_clear(Renderer *renderer) {
	if (!renderer) return;
	clear();
}


/*
	density_to_char
	takes the density and returns a coresponding char
	@return char - the char returned 
	@param density - the density to get the coresponding char from
*/
char density_to_char(float density) {
	if (density < 0.1f) return ' ';      
	else if (density < 0.3f) return '.'; 
	else if (density < 0.6f) return ':';   
	else if (density < 0.9f) return '#'; 
	else return '@'; 
}

int density_to_color(float density) {
	if (density < 0.2f) return 1;
	if (density < 0.5f) return 2;
	else return 3;
}

void draw_status(Renderer *renderer, const char *status) {
	if (!renderer || !status) return;

	if (renderer->use_colors) {
		attron(COLOR_PAIR(4));
		mvprintw(0, 0,"%s", status);
		attroff(COLOR_PAIR(4));
	} else {
		mvprintw(0, 0, "%s", status);
	}
}


/*
	renderer_draw_fluid
	draws the fluid to the screen
	@param *renderer - the renderer to get drawing data from 
	@param *fluid - the FluidSystem to draw 
*/
void renderer_draw_fluid(Renderer *renderer, FluidSystem *fluid) {
	if (!renderer || !fluid) return;

	for (int y = 0; y < renderer->height && y < fluid->height; y++) {
		for (int x = 0; x < renderer->width && x < fluid->width; x++) {
			float density = grid_get(fluid->density, x, y);

			if (density < renderer->density_treshold) {
				mvaddch(y, x, ' ');
				continue;
			}

			char ch = density_to_char(density);
			if (renderer->use_colors) {
				int COLOR_PAIR = density_to_color(density);
				attron(COLOR_PAIR);
				mvaddch(y, x, ch);
				attroff(COLOR_PAIR);
			} else {
				mvaddch(y, x, ch);
			}
		}
	}
}

/*
	renderer_present
	presents the rendered renderer
	@param renderer - to ensure it exsists
*/
void renderer_present(Renderer *renderer) {
	if (!renderer) return;
	refresh();
}

#endif
