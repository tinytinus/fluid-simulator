
#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>
#include "config.h"
#include "fluid.h"

/*
	InputState
	stores the current state of the input handeling 
	@param mouse_x - the mouse x position
	@param mouse_y - the mouse y position
	@param mouse_pressed - checks if the mouse is pressed 
	@param show_debug - stores wheter to show debug information 
	@param paused - stores wheter the simulation is currently running or paused 
*/
typedef struct {
    int mouse_x, mouse_y;
    bool mouse_pressed;
    bool show_debug;
    bool paused;
} InputState;

/*
	input_create
	creates a InputState to store the state 
	@return InputState* - returns the created InputState 
*/
InputState* input_create(void) {
	InputState *input = malloc(sizeof(InputState));
	if (!input) return NULL;

	input->mouse_x = 0;
	input->mouse_y = 0;
	input->mouse_pressed = false;
	input->paused = false;
	input->show_debug = false;

	mousemask(BUTTON1_PRESSED | BUTTON1_RELEASED | BUTTON1_CLICKED, NULL);

	return input;
}

/*
	input_destroy
	destroys a InputState
	@param *input - the inputstate to destroy 
*/
void input_destroy(InputState *input) {
	if (!input) return ;

	free(input);
} 

/*
	screen_to_grid
	maps screen positons to positions in the grid 
	if everything goes well this should always map 1:1
	@param screen_x - the position on the screen in the x axis
	@param screen_y - the position on the screen in the y axis 
	@param *grid_x - the position on the grid in the x axis
	@param *grid_y - the position on the screen in the y axis 
*/
void screen_to_grid(int screen_x, int screen_y, int *grid_x, int *grid_y) {
	if (grid_x) *grid_x = screen_x;
	if (grid_y) *grid_y = screen_y;

	if (*grid_x < 0) *grid_x = 0;
	if (*grid_x >= GRID_WIDTH) *grid_x = GRID_WIDTH - 1;

	if (*grid_y < 0) *grid_y = 0;
	if (*grid_y >= GRID_HEIGHT) *grid_y = GRID_HEIGHT - 1;
}

/*
	handle_mouse_input
	handles the mouse inputs like cliking 
	@param *input - the InputState to get the state from 
	@param *fluid - the fluid to affect 
*/
void handle_mouse_input(InputState *input, FluidSystem *fluid) {
	MEVENT event;

	if (getmouse(&event) == OK) {
		int grid_x, grid_y;
		screen_to_grid(event.x, event.y, &grid_x, &grid_y);

		input->mouse_x = grid_x;
		input->mouse_y = grid_y;

		if (event.bstate & BUTTON1_PRESSED) {
			input->mouse_pressed = true;
		}
		if (event.bstate & BUTTON1_RELEASED) {
			input->mouse_pressed = false;
		} 

		if (input->mouse_pressed) {
			fluid_add_density(fluid, grid_x, grid_y, FLUID_ADD);
		}
	}
}

/*
	handle_keyboard_input
	handles the keyboard input 
	@param *input - the InputState to get the state from 
	@param *fluid - the fluid to affect
	@param  ch - the charchter that got inputted 
*/
void handle_keyboard_input(InputState *input, FluidSystem *fluid, int ch) {
	switch (ch) {
		case 'r':
		case 'R':
			fluid_reset(fluid);
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

/*
	input_handle
	handles the input by sending it to the individual input handelers
	@param *input - the InputState to get the state from and give 
	@param *fluid - the fluid to affect 
*/
bool input_handle(InputState *input, FluidSystem *fluid) {
	if (!input || !fluid) return false;

	int ch = getch();

	if (ch != ERR) {
		handle_keyboard_input(input, fluid, ch);

		if (ch == 'q' || ch == 'Q') {
			return false;
		}
	}

	handle_mouse_input(input, fluid);

	return true;
}


#endif

