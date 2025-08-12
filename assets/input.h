
#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

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

	bool step_mode;
	bool step_once;
} InputState;

/*
	input_create
	creates a InputState to store the state 
	@return InputState* - returns the created InputState 
*/
InputState* input_create(void);

/*
	input_destroy
	destroys a InputState
	@param *input - the inputstate to destroy 
*/
void input_destroy(InputState *input);

/*
	screen_to_grid
	maps screen positons to positions in the grid 
	if everything goes well this should always map 1:1
	@param screen_x - the position on the screen in the x axis
	@param screen_y - the position on the screen in the y axis 
	@param *grid_x - the position on the grid in the x axis
	@param *grid_y - the position on the screen in the y axis 
*/
void screen_to_grid(int screen_x, int screen_y, int *grid_x, int *grid_y);

/*
	handle_mouse_input
	handles the mouse inputs like cliking 
	@param *input - the InputState to get the state from 
	@param *fluid - the fluid to affect 
*/
void handle_mouse_input(InputState *input, FluidSystem *fluid);

/*
	handle_keyboard_input
	handles the keyboard input 
	@param *input - the InputState to get the state from 
	@param *fluid - the fluid to affect
	@param  ch - the charchter that got inputted 
*/
void handle_keyboard_input(InputState *input, FluidSystem *fluid, int ch);

/*
	input_handle
	handles the input by sending it to the individual input handelers
	@param *input - the InputState to get the state from and give 
	@param *fluid - the fluid to affect 
*/
bool input_handle(InputState *input, FluidSystem *fluid);

#endif

