
#ifndef RENDERER_H
#define RENDERER_H

#include <ncurses.h>
#include "fluid.h"

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
	Renderer *renderer = malloc(sizeof(Renderer));
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

// Main rendering functions
void renderer_clear(Renderer *renderer) {

}

void renderer_draw_fluid(Renderer *renderer, FluidSystem *fluid);
void renderer_draw_velocity(Renderer *renderer, FluidSystem *fluid);
void renderer_present(Renderer *renderer);

// Utility functions
char density_to_char(float density) {
	if (density < 0.1f) return ' ';      
	else if (density < 0.3f) return '.'; 
	else if (density < 0.6f) return ':';   
	else if (density < 0.9f) return '#'; 
	else return '@'; 
}

int density_to_color(float density);
void draw_status(Renderer *renderer, const char *status);

#endif
