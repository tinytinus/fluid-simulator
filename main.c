#include <ncurses.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#include "assets/fluid.h"
#include "assets/renderer.h" 
#include "assets/input.h"
#include "assets/config.h"

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

	timeout(0);
    cbreak();

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

		draw_status(render, fluid, input);

		sleep(1 / 60);
    }
    
    // Cleanup
	
	fluid_destroy(fluid);
    renderer_destroy(render);

    endwin();
    return 0;
}
