#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "fluid.h"
#include "input.h"
#include "renderer.h"

int main() {
	printf("Testing malloc\n");

	// FluidSystem
	FluidSystem *fluid = fluid_create(10, 10);
	if (!fluid) {
		printf("FAIL: fluid_create\n");
		return 1;
	}
	printf("PASS: fluid_create\n");

	fluid_destroy(fluid); // this should throw a error if it fails 
	printf("PASS: fluid_destroy\n");

	//Grid2D
	Grid2D *grid = grid_create(5, 5);
    if (!grid) { 
		printf("FAIL: grid_create\n"); 
		return 1; 
	}
    printf("PASS: grid_create\n");

    grid_destroy(grid);
    printf("PASS: grid_destroy\n");

	// Test renderer
    Renderer *renderer = renderer_create(80, 24);
    if (!renderer) { 
		printf("FAIL: renderer_create\n"); 
		return 1; 
	}
    printf("PASS: renderer_create\n");
    
	renderer_destroy(renderer);
    printf("PASS: renderer_destroy\n");
    
    // Test input
    InputState *input = input_create();
    if (!input) { 
		printf("FAIL: input_create\n"); 
		return 1; 
	}
    printf("PASS: input_create\n");
    
	input_destroy(input);
    printf("PASS: input_destroy\n");

	return 0;
}
