#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <SDL2/SDL.h>

#undef main

#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

#define SW 300
#define SH 300

SDL_Window* win;
SDL_Renderer* rend;
SDL_Event event;

#define Point SDL_Point

typedef struct{
	float x, y, z;
}Vec3;

typedef struct{
	Uint8 r, g, b;
} Color;

Vec3 vsub(Vec3 a, Vec3 b){
	return (Vec3){a.x-b.x,a.y-b.y,a.z-b.z};
}

float dot(Vec3 a, Vec3 b){
	return (a.x*b.x + a.y*b.y);
}

Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p){
	Vec3 v0 = vsub(b,a), v1 = vsub(c, a), v2 = vsub(p,a);
	float d00 = dot(v0,v0);
	float d01 = dot(v0,v1);
	float d11 = dot(v1,v1);
	float d20 = dot(v2,v0);
	float d21 = dot(v2,v1);
	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.f - v - w;
	return (Vec3){u,v,w};
}

float interpolate_depth(float a, float b, float c, Vec3 p){
	return (a*p.x+b*p.y+c*p.z);
}

Color interpolate_colors(Vec3 a, Vec3 b, Vec3 c, Vec3 p){
	return (Color){
		(Uint8)(a.x*p.x+b.x*p.y+c.x*p.z),
		(Uint8)(a.y*p.x+b.y*p.y+c.y*p.z),
		(Uint8)(a.z*p.x+b.z*p.y+c.z*p.z)
	};
}

typedef struct{
	float a, b;
}Function;

typedef struct{
	Vec3 v[3];
	Vec3 c[3];
}tri;

tri t1 = (tri){
	.v=(Vec3[3]){
		(Vec3){0.f,0.5f,1.f},
		(Vec3){-0.5f,-0.5f,5.f},
		(Vec3){0.5f,-0.5f,1.f}
	},
	.c=(Vec3[3]){
		(Vec3){255.f,0.f,0.f},
		(Vec3){0.f,255.f,0.f},
		(Vec3){0.f,0.f,255.f}
	}
};

bool init(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
		printf("SDL_Init error: %s\n",SDL_GetError());
		return false;
	}
	win = SDL_CreateWindow("Hello Triangle!",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,SW,SH,SDL_WINDOW_SHOWN);
	if(win == NULL){
		printf("Window creation error: %s\n",SDL_GetError());
		return false;
	}
	rend = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);
	if(rend == NULL){
		printf("Renderer creation error: %s\n",SDL_GetError());
		return false;
	}
	return true;
}

void quit_everything(){
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
	exit(0);
}

Function getfunc(Vec3 x, Vec3 y){
	float deltaX = x.x-y.x;
	float a = (deltaX != 0)?(deltaX/(x.y-y.y)):(100000.f);
	float b = x.x-a*x.y;
	return (Function){a,b};
}

Function minmax(Vec3 x, Vec3 y){
	return (Function){
		min(x.x,y.x), max(x.x,y.x)
	};
}

float func(Function f, float y){
	return f.a*y+f.b;
}

Vec3 project_v3(Vec3 v){
	if(v.z <= 0.f) return (Vec3){0.f,0.f,v.z};
	return (Vec3){v.x/v.z, v.y/v.z, v.z};
}

tri project_triangle(tri* t){
	tri result = (tri){
		.v={
			(Vec3)project_v3(t->v[0]),
			(Vec3)project_v3(t->v[1]),
			(Vec3)project_v3(t->v[2])
		},
		.c={
			(Vec3)t->c[0],
			(Vec3)t->c[1],
			(Vec3)t->c[2]
		}
	};
	return result;
}

void render_triangle(tri* t){
	tri pt = project_triangle(t);
	Function edges[3]={
		getfunc(pt.v[0],pt.v[1]),
		getfunc(pt.v[1],pt.v[2]),
		getfunc(pt.v[2],pt.v[0])
	};
	Function edge_lim[3]={
		minmax(pt.v[0],pt.v[1]),
		minmax(pt.v[1],pt.v[2]),
		minmax(pt.v[2],pt.v[0])
	};
	int ymin = (int)floor((min(t->v[0].y,min(t->v[1].y,t->v[2].y))+0.5)*(float)SH);
	int ymax = (int)ceil((max(t->v[0].y,max(t->v[1].y,t->v[2].y))+0.5)*(float)SH);
	for(int y = ymin; y < ymax; y++){
		float sline[2]; int sline_len=0;
		for(int i = 0; i < 3 && sline_len < 2; i++){
			float x = func(edges[i],-((float)y/(float)SH-0.5f));
			if(x >= edge_lim[i].a && x < edge_lim[i].b)
				if(sline_len == 0 || (sline_len > 0 && round(sline[sline_len-1]*1000) != round(x*1000))) sline[sline_len++] = x;
		}
		if(sline_len == 2){
			int xmin = (int)floor((min(sline[0],sline[1])+0.5f)*(float)SW);
			int xmax = (int)ceil((max(sline[0],sline[1])+0.5f)*(float)SW);
			for(int i=xmin; i < xmax; i++){
				Vec3 bc = barycentric(t->v[0],t->v[1],t->v[2],(Vec3){((float)i/(float)SW-0.5f),-((float)y/(float)SH-0.5f),0.f});
				Color clr = interpolate_colors(t->c[0],t->c[1],t->c[2],bc);
				SDL_SetRenderDrawColor(rend,clr.r,clr.g,clr.b,255);
				SDL_RenderDrawPoint(rend,i,y);
			}
		}
	}
}

int main(int argc, char* argv[]){
	if(!init()) return 1;
	
	//Uint32 NOW=SDL_GetTicks(), LAST=0;
	while(true){
		//LAST = NOW;
		//NOW = SDL_GetTicks();
		//float FPS = 1000.f/(float)(NOW-LAST);
		//printf("FPS: %.1f\n",FPS);

		while(SDL_PollEvent(&event) != 0){
			if(event.type == SDL_QUIT)
				quit_everything();
		}
		SDL_SetRenderDrawColor(rend,0,0,0,0);
		SDL_RenderClear(rend);

		render_triangle(&t1);

		SDL_RenderPresent(rend);
	}

	quit_everything();
}