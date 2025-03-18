#ifndef TRIANGLE_RASTER_H
#define TRIANGLE_RASTER_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Math functions
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// Boolean values
#define bool _Bool
#define true 1
#define false 0


// ------------------

#if defined(TRIANGLE_RASTER_SDL)
// Will render the triangles with SDL2

// Resolution of the SDL Window (in pixels)
#define SW 400
#define SH 400

// Window and renderer
SDL_Window* win = NULL;
SDL_Renderer* rend = NULL;
SDL_Event event;

// Initialize SDL2
bool init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL_Init error: %s\n",SDL_GetError());
		return false;
	}
	win = SDL_CreateWindow("Triangle Rasterizer",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SW,SH,SDL_WINDOW_SHOWN);
	if(window == NULL){
		printf("SDL_CreateWindow error: %s\n",SDL_GetError());
		return false;
	}
	rend = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
	if(rend == NULL){
		printf("SDL_CreateRenderer error: %s\n",SDL_GetError());
		return false;
	}
	return true;
}

// Quits libraries
void quit_everything(){
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	exit(0);
}

// Clear the screen
void clear_screen(){
	SDL_SetRenderDrawColor(rend,0,0,0,255);
	SDL_RenderClear(rend);
}

// Update the screen
void update_screen(){
	SDL_RenderPresent(rend);
}


// ------------------

#elif defined(TRIANGLE_RASTER_TERMINAL)
// Will render the triangles on the terminal using ASCII characters

// Resoltution
#define SW 40
#define SH 30

// Screen buffer for characters to be rendered to the screen
char screen_buffer[SH][SW+1];

// Initialize the screen buffer
bool init(){
	memset(screen_buffer,0,SH*(SW+1));
	return true;
}

// Just exit the program
void quit_everything(){
	exit(0);
}

// Clear the screen
#if defined(_WIN32)
void clear_screen(){ system("cls"); }
#elif defined(__linux__)
void clear_screen(){ printf("\e[2J\e[H"); }
#endif

// ---------------------------

#else
#error "Please define TRIANGLE_RASTER_SDL or TRIANGLE_RASTER_TERMINAL to chose what rendering interface to use."
#endif

#endif