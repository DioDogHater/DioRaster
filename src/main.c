#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

// Draw in terminal, with faces winded in ClockWise order
#define DIO_RASTER_TERMINAL
#define DIO_RASTER_CW

// Uncomment to enable SDL
//#define DIO_RASTER_SDL

// Window dimensions (if SDL is turned off)
#ifdef DIO_RASTER_TERMINAL
#define SW 125	// Screen width
#define SH 62	// Screen height
#endif

// Experimental
//#define DIO_RASTER_MULTITHREADING

// Comment for grayscale
//#define DIO_RASTER_FULL_COLOR

// Best settings for grayscale
#ifndef DIO_RASTER_FULL_COLOR
#define DIO_RASTER_AMBIENT 0.1f
#define DIO_RASTER_DIFFUSE 0.85f

// Best settings for Full Color mode
#else
#define DIO_RASTER_AMBIENT 0.3f
#define DIO_RASTER_DIFFUSE 0.7f
#endif

// The triange rasterization library
#include "dioraster.h"

// The Wavefront (.obj) parsing library
#include "objparser.h"

DR_Mesh load_mesh(char* path, DR_Color c){
	// Parse the scene
	OBJ_Data obj_data;
	if(!OBJ_parse(&obj_data,path)) exit(1);

	// Load mesh from the scene
	DR_Mesh result = OBJ_data_to_mesh(&obj_data,c);

	// Free the scene
	OBJ_free_data(&obj_data);
	return result;
}

int main(int argc, char* argv[]){
	// Load the car mesh
	DR_Mesh car_mesh = load_mesh("car.obj",(DR_Color){255,125,50});
	car_mesh.pos = (Vec3){-0.5f,0.f,5.f};

	// Load the suzanne mesh
	DR_Mesh suzanne_mesh = load_mesh("suzanne.obj",(DR_Color){255,255,255});
	suzanne_mesh.pos = (Vec3){0.5f,0.f,3.f};

	// Initialize the rasterization libary
	if(!DR_init()) return 1;
	
	// Set the light source position to {5,10,-10}
	DR_light = (Vec3){10,10,-10};

	// Loop
	float frames = 0.f;
	while(true){
		// Handle events if we are using SDL2 to close the window if necessary
		DR_handle_events();

		DR_clear_screen();

		// Render the car and suzanne with shading
		DR_render_mesh_shaded(suzanne_mesh);
		DR_render_mesh_shaded(car_mesh);

		// Rotate the car
		car_mesh.rot.y += 0.02f;
		car_mesh.rot.x = 0.5f*sin(0.75f*frames);

		// Rotate the suzanne mesh
		suzanne_mesh.rot.x += 0.03f;
		suzanne_mesh.rot.z += 0.02f;
		suzanne_mesh.pos.y = 0.5f*sin(frames);

		// Update the screen
		DR_update_screen();
		frames += 0.01f;
		
		// Wait 2ms between each frame (uncomment if the FPS are too high)
		//sleepms(2);
	}

	// Free the mesh we loaded at the start
	OBJ_free_mesh(&car_mesh);
	OBJ_free_mesh(&suzanne_mesh);

	DR_quit_everything();
}
