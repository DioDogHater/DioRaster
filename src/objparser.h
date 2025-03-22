#ifndef OBJ_PARSER_H
#define OBJ_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// This header file is a .obj (Wavefront format) file parser
// Please read the comments for further detail

// For now, supports only position vertices and face information
// Might add texture support later in development

// Scene information structs
typedef struct {
	int v[3];
} OBJ_Face;

// Struct holding the data of a parsed .obj file
typedef struct {
	Vec3* vertices;
	int vertex_count;
	OBJ_Face* faces;
	int face_count;
} OBJ_Data;

// Vector-like function utilities
void OBJ_pushback_vertex(OBJ_Data* data, Vec3 vertex){
	data->vertices=(Vec3*)realloc(data->vertices,++data->vertex_count*sizeof(Vec3));
	data->vertices[data->vertex_count-1]=vertex;
}
void OBJ_pushback_face(OBJ_Data* data, OBJ_Face face){
	data->faces=(OBJ_Face*)realloc(data->faces,++data->face_count*sizeof(OBJ_Face));
	data->faces[data->face_count-1]=face;
}

// Global variables
size_t obj_pos;
size_t obj_size;
bool obj_success;

bool OBJ_parse_line(OBJ_Data* data, char* obj_buffer);

// Open a file, parse it and handle exceptions
bool OBJ_parse(OBJ_Data* data, char* path){
	// Open the file in binary read mode
	FILE* fptr = fopen(path,"rb");
	if(fptr == NULL){
		perror("Failed to open .obj file!\n");
		return false;
	}
	// Determine the entire size of the file to allocate a buffer
	fseek(fptr,0L,SEEK_END);
	obj_size = ftell(fptr);
	rewind(fptr); // Goes back to the beginning of the file
	// Allocate the buffer
	char* obj_buffer = (char*)malloc(obj_size+1);
	if(obj_buffer == NULL){
		printf("Failed to allocate buffer during .obj file parsing!\n");
		return false;
	}
	// Read the contents of the file
	fread(obj_buffer,1,obj_size,fptr);
	// Parse through the file
	obj_success = true;
	obj_pos = 0;
	*data = (OBJ_Data){NULL,0,NULL,0};
	while(OBJ_parse_line(data,obj_buffer)){
		//printf("current file position: %lu/%lu\n",obj_pos,obj_size);
	}

	// Free buffer
	free(obj_buffer);
	obj_pos = 0;
	obj_size = 0;
	return obj_success;
}

bool OBJ_parse_line(OBJ_Data* data, char* obj_buffer){
	if(obj_pos >= obj_size) return false;
	// Assess the line size
	size_t line_start = obj_pos;
	for(;obj_pos < obj_size && obj_buffer[obj_pos] != '\n'; obj_pos++);
	
	// If it is a single character, just skip it
	if(obj_pos-line_start == 0){
		obj_pos++;
		return true;
	}

	// Parse the line
	for(int i = line_start; i <= obj_pos; i++){
		if(isspace(obj_buffer[i])) continue;
		if(obj_buffer[i] == '#'){
			// If it's a comment
			obj_pos++;
			return true;
		}else if(obj_buffer[i] == 'v'){
			if(!isspace(obj_buffer[i+1])){
				obj_pos++;
				return true;
			} // In case it isn't a position vector...

			// This is a vertex, so store it in the vertex vector
			Vec3 v;
			if(sscanf(obj_buffer+i,"v %f %f %f",&v.x,&v.y,&v.z) < 3){
				printf("Failed to retrieve all components of vector in .obj file!\n->\"%s\"\n",&obj_buffer[i]);
				obj_pos++;
				obj_success = false;
				return false;
			}
			printf("Retrieved vector: %f, %f, %f\n",v.x,v.y,v.z);
			OBJ_pushback_vertex(data,v);
			obj_pos++;
			return true;
		}else if(obj_buffer[i] == 'f'){
			int temp;
			// This is a face, so store it in the face vector
			OBJ_Face f;
			if(sscanf(obj_buffer+i,"f %d %d %d",&f.v[0],&f.v[1],&f.v[2]) == 3){
				printf("Retrieved face: %d, %d, %d\n",f.v[0],f.v[1],f.v[2]);
				OBJ_pushback_face(data,f);
				obj_pos++;
				return true;
			}// Check if maybe it uses the "f a/b c/d e/f" format
			else if(sscanf(obj_buffer+i,"f %d/%d %d/%d %d/%d",&f.v[0],&temp,&f.v[1],&temp,&f.v[2],&temp) == 6){
				printf("Retrieved face: %d, %d, %d\n",f.v[0],f.v[1],f.v[2]);
				OBJ_pushback_face(data,f);
				obj_pos++;
				return true;
			}
			// Check if maybe it uses the "f a/b/c d/e/f g/h/i" format
			else if(sscanf(obj_buffer+i,"f %d/%d/%d %d/%d/%d %d/%d/%d",&f.v[0],&temp,&temp,&f.v[1],&temp,&temp,&f.v[2],&temp,&temp) == 9){
				printf("Retrieved face: %d, %d, %d\n",f.v[0],f.v[1],f.v[2]);
				OBJ_pushback_face(data,f);
				obj_pos++;
				return true;
			}else{
				printf("Failed to retrieve all indexes of face in .obj file!\n->\"%s\"\n",&obj_buffer[i]);
				obj_pos++;
				obj_success = false;
				return false;
			}
			
		}
	}
	obj_pos++;
	return true;
}

// Free resources allocated by the OBJ_Data structure
void OBJ_free_data(OBJ_Data* data){
	if(data->vertices != NULL) free(data->vertices);
	data->vertices = NULL;
	data->vertex_count = 0;
	if(data->faces != NULL) free(data->faces);
	data->faces = NULL;
	data->face_count = 0;
}

// ---------------- TRANSFORM OBJ_DATA INTO MESH -----------------------

Mesh OBJ_data_to_mesh(OBJ_Data* data, Color default_color){
	if(data->face_count < 1 && data->vertex_count < 1) // If the model is "empty"
		return (Mesh){NULL,0,(Vec3){0.f,0.f,0.f},(Vec3){0.f,0.f,0.f}};
	// Allocate resources necessary to load model and initialize pos and rot
	Mesh m;
	m.triangles = (Triangle*)malloc(sizeof(Triangle)*data->face_count);
	m.triangle_count = data->face_count;
	//printf("Triangle count: %d\n",m.triangle_count);
	m.pos = (Vec3){0.f,0.f,0.f};
	m.rot = (Vec3){0.f,0.f,0.f};

	// Now parse through all faces and add them to the mesh
	for(int i = 0; i < data->face_count; i++){
		Triangle tri;
		//printf("Triangle %d:\n",i);
		for(int j = 0; j < 3; j++){
			// Check if the vertex doesn't exist
			// If it doesn't exist, just throw an error and return empty mesh
			if(data->faces[i].v[j] > data->vertex_count){
				free(m.triangles);
				m.triangles = NULL;
				m.triangle_count = 0;
				printf("Error: Face in OBJ_Data appears to reference non-existant vertex!\n");
				return m;
			}
			tri.v[j] = data->vertices[data->faces[i].v[j]-1];
			// Give every vertex the same color (Might change that later ;) )
			tri.c[j] = (Vec3){(float)default_color.r,(float)default_color.g,(float)default_color.b};
			printf("%f, %f, %f\n",tri.v[j].x,tri.v[j].y,tri.v[j].z);
		}
		m.triangles[i] = tri;
	}
	return m;
}

void OBJ_free_mesh(Mesh* m){
	if(m->triangles != NULL) free(m->triangles);
	m->triangles = NULL;
	m->triangle_count = 0;
}


#endif