#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

#include "fluid.h"
#include "renderer.h" 
#include "input.h"
#include "config.h"

// Global variables for cleanup
static bool running = true;

// Signal handler for graceful shutdown
void cleanup_handler(int sig) {
    running = false;
}

int main() {
    // Setup signal handler
    signal(SIGINT, cleanup_handler);
    
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // Initialize colors if supported
    if (has_colors()) {
        start_color();
        use_default_colors();
        // Color pair initialization will go here
    }
    
    // Initialize simulation components
    FluidSystem *fluid = fluid_create(GRID_WIDTH, GRID_HEIGHT);
    Renderer *render = renderer_create(GRID_WIDTH, GRID_HEIGHT);
    InputState *input = input_create();

	// TODO: Set initial conditions
    
    // Main simulation loop
    while (running) {
    	if (!input_handle(input, fluid)) break;

		if (!input->paused) fluid_update(fluid);
        
		renderer_clear(render);
		renderer_draw_fluid(render, fluid);
		renderer_present(render);
        
		// TODO: Frame rate limiting
        
		usleep(16667); // ~60 FPS
    }
    
    // Cleanup
	
	fluid_destroy(fluid);
    renderer_destroy(render);

    endwin();
    return 0;
}
