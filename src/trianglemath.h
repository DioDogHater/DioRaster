#ifndef TRIANGLE_MATH_H
#define TRIANGLE_MATH_H

// Custom min and max macros
#define min(a,b) ((a)<(b)?(a):(b))
#define max(a,b) ((a)>(b)?(a):(b))

// 3D vector
typedef struct {
	float x, y, z;
} Vec3;

// Structure that holds 3 vertices, v = position, c = color
typedef struct {
	Vec3 v[3];
	Vec3 c[3];
} Triangle;

typedef struct {
	Triangle* triangles;
	int triangle_count;
	Vec3 pos;
	Vec3 rot;
} Mesh;

// Structure used in rendering step
struct TriangleBounds {
	int xmin, xmax; // Bounds of the triangle (to avoid checking every single pixel)
	int ymin, ymax;
};

// ----------------------- VECTOR MATH -------------------------------

Vec3 add_Vec3(Vec3 a, Vec3 b){
	return (Vec3){
		a.x+b.x, a.y+b.y, a.z + b.z
	};
}

Vec3 sub_Vec3(Vec3 a, Vec3 b){
	return (Vec3){
		a.x-b.x, a.y-b.y, a.z-b.z
	};
}

// ---------------- RENDERING UTILITY --------------------------------

// Get the TriangleBounds of the triangle
struct TriangleBounds get_triangle_bounds(Triangle t){
	struct TriangleBounds bounds = (struct TriangleBounds){
		(int)max((min(t.v[0].x,min(t.v[1].x,t.v[2].x))+0.5f)*(float)SW,0),
		(int)min((max(t.v[0].x,max(t.v[1].x,t.v[2].x))+0.5f)*(float)SW,SW),
		(int)max((min(-t.v[0].y,min(-t.v[1].y,-t.v[2].y))+0.5f)*(float)SH,0),
		(int)min((max(-t.v[0].y,max(-t.v[1].y,-t.v[2].y))+0.5f)*(float)SH,SH)
	};
	return bounds;
};

// Turns 2D integer pixel position to a -0.5 to 0.5 ranged 2D vector
Vec3 px_to_screen_space(int x, int y){
	float nx = (float)x+0.5f;
	float ny = (float)y+0.5f;
	return (Vec3){nx/(float)SW-0.5f, -(ny/(float)SH-0.5f)};
}

// Rotate 3D vector with Rz, Ry and Rx matrices in order
Vec3 rotateZYX(Vec3 v, Vec3 rot){
	Vec3 result = v;
	if(rot.z != 0.f){
		result.x = v.x*cos(rot.z)-v.y*sin(rot.z);
		result.y = v.x*sin(rot.z)+v.y*cos(rot.z);
		v.x = result.x;
		v.y = result.y;
	}
	if(rot.y != 0.f){
		result.x = v.x*cos(rot.y)-v.z*sin(rot.y);
		result.z = v.x*sin(rot.y)+v.z*cos(rot.y);
		v.x = result.x;
		v.z = result.z;
	}
	if(rot.x != 0.f){
		result.z = v.z*cos(rot.x)-v.y*sin(rot.x);
		result.y = v.z*sin(rot.x)+v.y*cos(rot.x);
	}
	return result;
}

// Transform vertex with translation and rotation
Triangle transform_triangle(Triangle t, Vec3 pos, Vec3 rot){
	for(int i = 0; i < 3; i++){
		t.v[i] = rotateZYX(t.v[i],rot);
		t.v[i] = add_Vec3(t.v[i],pos);
	}
	return t;
}

Triangle project_triangle(Triangle t){
	for(int i = 0; i < 3; i++){
		t.v[i] = (Vec3){
			t.v[i].x/t.v[i].z, t.v[i].y/t.v[i].z, t.v[i].z
		};
	}
	return t;
}

// ---------------- RASTERIZATION MATH -------------------------------

// Get the result of the edge function between 2 points of a triangle and a point P
// This function only uses the x and y components since rendering is done after 3D projection
float edge_function(Vec3 a, Vec3 b, Vec3 p){
	return (p.x-a.x)*(b.y-a.y) - (p.y-a.y)*(b.x-a.x);
}

// NOW THIS IS IMPORTANT! AT THIS STAGE, VERTEX WINDING ORDER IS IMPORTANT
// Please use define TRIANGLE_RASTER_CCW to use counter-clockwise
// Please use define TRIANGLE_RASTER_CW to use clockwise
#if defined(TRIANGLE_RASTER_CCW)
#define RASTER_ORDER_CMP <=
#elif defined(TRIANGLE_RASTER_CW)
#define RASTER_ORDER_CMP >=
#else
#error "Please either define TRIANGLE_RASTER_CW or TRIANGLE_RASTER_CCW before including triangleraster.h"
#endif

// Will give you the barycentric coordinates if point P is inside the triangle
// Otherwise will give you a Vector3 whose x component is negative
Vec3 barycentric(Vec3 a, Vec3 b, Vec3 c, Vec3 p){
	float w0 = edge_function(a,b,p);
	float w1 = edge_function(b,c,p);
	float w2 = edge_function(c,a,p);
	if(w0 RASTER_ORDER_CMP 0 && w1 RASTER_ORDER_CMP 0 && w2 RASTER_ORDER_CMP 0){
		float area = edge_function(a,b,c);
		w0 /= area;
		w1 /= area;
		w2 /= area;
		return (Vec3){w0,w1,w2};
	}
	return (Vec3){-1.f,0.f,0.f};
}

// Interpolate the depth (Z component) of triangles' vertices
// Used for depth testing
float interpolate_depth(Vec3 a, Vec3 b, Vec3 c, Vec3 p){
	return (a.z*p.x) + (b.z*p.y) + (c.z*p.z);
}

// Interpolate colors
Color interpolate_colors(Vec3 a, Vec3 b, Vec3 c, Vec3 p){
	return (Color){
		(int)((a.x*p.x) + (b.x*p.y) + (c.x*p.z)),
		(int)((a.y*p.x) + (b.y*p.y) + (c.y*p.z)),
		(int)((a.z*p.x) + (b.z*p.y) + (c.z*p.z))
	};
}

#endif