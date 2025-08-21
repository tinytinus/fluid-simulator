#include <ncurses.h>
#include <signal.h>
#include <stdbool.h>
#include <sys/select.h>
#include <unistd.h>

#include "assets/config.h"
#include "assets/fluid.h"
#include "assets/input.h"
#include "assets/renderer.h"

// Global variables for cleanup
static bool running = true;

// Signal handler for graceful shutdown
void cleanup_handler(int sig) {
  (void)sig;
  running = false;
}

int main() {
  // Setup signal handler
  signal(SIGINT, cleanup_handler);

  // Initialize ncurses
  initscr();
  nodelay(stdscr, TRUE);
  cbreak();
  noecho();
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
    if (!input_handle(input, fluid))
      break;

    bool should_update = !input->paused;
    if (input->step_mode && input->step_once) {
      should_update = true;
      input->step_once = false;
    }
    if (should_update)
      fluid_update(fluid);

    renderer_clear(render);
    renderer_draw_fluid(render, fluid);

    if (input->show_debug) {
      draw_status(render, fluid, input);
    }

    renderer_present(render);

    // TODO: Frame rate limiting
    struct timeval tv = {0, 16667}; // 16.67ms in microseconds
    select(0, NULL, NULL, NULL, &tv);
  }

  // Cleanup

  fluid_destroy(fluid);
  renderer_destroy(render);
  input_destroy(input);

  endwin();
  return 0;
}
