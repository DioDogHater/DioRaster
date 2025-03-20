#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define TRIANGLE_RASTER_TERMINAL
#define TRIANGLE_RASTER_CCW
#include "triangleraster.h"

Mesh test_mesh = (Mesh){
	(Triangle[]){
		// Front
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,-0.5f},(Vec3){-0.5f,-0.5f,-0.5f},(Vec3){0.5f,-0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,-0.5f},(Vec3){0.5f,-0.5f,-0.5f},(Vec3){0.5f,0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		// Back
		(Triangle){
			.v={(Vec3){0.5f,0.5f,0.5f},(Vec3){0.5f,-0.5f,0.5f},(Vec3){-0.5f,-0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){0.5f,0.5f,0.5f},(Vec3){-0.5f,-0.5f,0.5f},(Vec3){-0.5f,0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		// Left
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,0.5f},(Vec3){-0.5f,-0.5f,0.5f},(Vec3){-0.5f,-0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,0.5f},(Vec3){-0.5f,-0.5f,-0.5f},(Vec3){-0.5f,0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		// Right
		(Triangle){
			.v={(Vec3){0.5f,0.5f,-0.5f},(Vec3){0.5f,-0.5f,-0.5f},(Vec3){0.5f,-0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){0.5f,0.5f,-0.5f},(Vec3){0.5f,-0.5f,0.5f},(Vec3){0.5f,0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		// Top
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,0.5f},(Vec3){-0.5f,0.5f,-0.5f},(Vec3){0.5f,0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){-0.5f,0.5f,0.5f},(Vec3){0.5f,0.5f,-0.5f},(Vec3){0.5f,0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		// Bottom
		(Triangle){
			.v={(Vec3){0.5f,-0.5f,0.5f},(Vec3){0.5f,-0.5f,-0.5f},(Vec3){-0.5f,-0.5f,-0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		},
		(Triangle){
			.v={(Vec3){0.5f,-0.5f,0.5f},(Vec3){-0.5f,-0.5f,-0.5f},(Vec3){-0.5f,-0.5f,0.5f}},
			.c={(Vec3){255.f,0.f,0.f},(Vec3){0.f,255.f,0.f},(Vec3){0.f,0.f,255.f}}
		}
	},
	12,
	(Vec3){0.f,0.f,3.f},
	(Vec3){0.f,0.f,0.f}
};

int main(int argc, char* argv[]){
	if(!init()) return 1;
	
	while(true){
		handle_events();

		clear_screen();
		
		render_mesh(test_mesh);
		test_mesh.rot.y += 0.005f;
		test_mesh.rot.x += 0.0025f;

		update_screen();
	}

	quit_everything();
}