
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>

#include "input.h"
#include "config.h"
#include "fluid.h"

InputState* input_create(void) {
	InputState *input = (InputState*)malloc(sizeof(InputState));
	if (!input) return NULL;

	input->mouse_x = 0;
	input->mouse_y = 0;
	input->mouse_pressed = false;
	input->paused = false;
	input->show_debug = false;

	mousemask(BUTTON1_PRESSED | BUTTON1_RELEASED | BUTTON1_CLICKED, NULL);

	return input;
}

void input_destroy(InputState *input) {
	if (!input) return ;

	free(input);
} 

void screen_to_grid(int screen_x, int screen_y, int *grid_x, int *grid_y) {
	if (grid_x) *grid_x = screen_x;
	if (grid_y) *grid_y = screen_y;

	if (*grid_x < 0) *grid_x = 0;
	if (*grid_x >= GRID_WIDTH) *grid_x = GRID_WIDTH - 1;

	if (*grid_y < 0) *grid_y = 0;
	if (*grid_y >= GRID_HEIGHT) *grid_y = GRID_HEIGHT - 1;
}

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
