
#ifndef INPUT_H
#define INPUT_H

#include <ncurses.h>
#include "fluid.h"

typedef struct {
    int mouse_x, mouse_y;
    bool mouse_pressed;
    bool show_velocity;
    bool paused;
} InputState;

// Input management
InputState* input_create(void);
void input_destroy(InputState *input);

// Main input processing
bool input_handle(InputState *input, FluidSystem *fluid);

// Specific input handlers
void handle_mouse_input(InputState *input, FluidSystem *fluid);
void handle_keyboard_input(InputState *input, FluidSystem *fluid, int ch);

// Coordinate conversion
void screen_to_grid(int screen_x, int screen_y, int *grid_x, int *grid_y);

#endif

