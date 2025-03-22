#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Draw in terminal, with faces winded in ClockWise order
#define TRIANGLE_RASTER_TERMINAL
#define TRIANGLE_RASTER_CW

// Window dimensions
#define SW 125
#define SH 62

// Rendering parameters
#define TRIANGLE_RASTER_SLEEPMS 2
#define TRIANGLE_RASTER_FULL_COLOR
#define TRIANGLE_RASTER_AMBIENT 0.5f
#define TRIANGLE_RASTER_DIFFUSE 0.5f

// The triange rasterization library
#include "triangleraster.h"

// The Wavefront (.obj) parsing library
#include "objparser.h"

int main(int argc, char* argv[]){
	// Parse the scene
	OBJ_Data obj_data;
	if(!OBJ_parse(&obj_data,"car.obj")) return 1;

	// Load mesh from the scene
	Mesh test_mesh = OBJ_data_to_mesh(&obj_data,(Color){255,150,25});
	test_mesh.pos = (Vec3){0.f,0.f,3.75f};

	// Free the scene
	OBJ_free_data(&obj_data);

	// Initialize the rasterization libary
	if(!init()) return 1;
	
	// Loop
	float frames = 0.f;
	while(true){
		// Handle events if we are using SDL2 to close the window if necessary
		handle_events();

		clear_screen();

		// Render the mesh with shading
		// The light source is at (10,10,0)
		render_mesh_shaded(test_mesh,(Vec3){10.f,10.f,0.f});

		// Rotate the car
		test_mesh.rot.y += 0.02f;
		test_mesh.rot.x = 0.5f*sin(0.75f*frames);

		// Update the screen
		update_screen();
		frames += 0.01f;
	}

	// Free the mesh we loaded at the start
	OBJ_free_mesh(&test_mesh);

	quit_everything();
}