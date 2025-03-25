
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

float dot_product(Vec3 a, Vec3 b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

#if defined(TRIANGLE_RASTER_FAST_INVSQRT)
// Fast inverse square root from Quake
float fast_invsqrt(float number){
	long i;
	float x2, y;
	const float threehalfs = 1.5f;

	x2 = number * 0.5f;
	y = number;
	i = *(long*)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float*)&i;
	y = y * (threehalfs - (x2*y*y));
	return y;
}
float inv_magnitude(Vec3 v){
	return fast_invsqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}
#else
float inv_magnitude(Vec3 v){
	return 1.f/(float)sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}
#endif

float magnitude(Vec3 v){
	return 1.f/inv_magnitude(v);
}

Vec3 normalize(Vec3 v){
	float m = inv_magnitude(v);
	if(m == 0) return v;
	return (Vec3){v.x*m,v.y*m,v.z*m};
}

Vec3 cross_product(Vec3 a, Vec3 b){
	return (Vec3){
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}

Vec3 get_normal(Vec3 a, Vec3 b, Vec3 c){
	Vec3 A = sub_Vec3(b,a);
	Vec3 B = sub_Vec3(c,a);
	return cross_product(A,B);
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

#ifndef TRIANGLE_RASTER_AMBIENT
#define TRIANGLE_RASTER_AMBIENT 0.25f
#endif
#ifndef TRIANGLE_RASTER_DIFFUSE
#define TRIANGLE_RASTER_DIFFUSE 0.75f
#endif

void shade_triangle(Triangle* t, Vec3 light){
	Vec3 norm = normalize(get_normal(t->v[0],t->v[1],t->v[2]));
	for(int i=0; i<3; i++){
		Vec3 lightDir = normalize(sub_Vec3(light,t->v[i]));
		float diff = max(dot_product(norm,lightDir),0.f);
		float intensity = (TRIANGLE_RASTER_AMBIENT+diff*TRIANGLE_RASTER_DIFFUSE);
		Vec3 diffuse = (Vec3){intensity*t->c[i].x,intensity*t->c[i].y,intensity*t->c[i].z};
		t->c[i] = diffuse;
	}
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
