#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define TRIANGLE_RASTER_TERMINAL
#define TRIANGLE_RASTER_CW
#define SW 150
#define SH 75
#define TRIANGLE_RASTER_SLEEPMS 0
#include "triangleraster.h"

#include "objparser.h"

int main(int argc, char* argv[]){
	if(!init()) return 1;

	OBJ_Data obj_data;
	if(!OBJ_parse(&obj_data,"car.obj")) return 1;

	Mesh test_mesh = OBJ_data_to_mesh(&obj_data);
	test_mesh.pos = (Vec3){0.f,0.f,3.5f};

	OBJ_free_data(&obj_data);
	printf("Parsed mesh!\n");
	
	float frames = 0.f;
	while(true){
		handle_events();

		clear_screen();

		render_mesh_shaded(test_mesh,(Vec3){50.f,50.f,-50.f});
		//render_mesh(test_mesh);
		test_mesh.rot.y += 0.02f;
		test_mesh.rot.x = 0.5f*sin(0.75f*frames);

		update_screen();
		frames += 0.01f;
	}

	OBJ_free_mesh(&test_mesh);

	quit_everything();
}