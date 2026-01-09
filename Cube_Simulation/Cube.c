// gcc -o Cube Cube.c `sdl2-config --cflags --libs`

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600

#define VSIZE 10
#define WHITE 0xffffffff

typedef struct { float x,y,z; } Vector3;
typedef struct { float x,y; } Vector2;
typedef struct { Vector3 *vertices; } Cube; // Cube should have 8 vertices

void init(Cube *cube) {
	cube->vertices = malloc(sizeof(Vector3) * 8);
	cube->vertices[0] = (Vector3) {-0.5, -0.5, -0.5};
    	cube->vertices[1] = (Vector3) {-0.5, -0.5,  0.5};
    	cube->vertices[2] = (Vector3) {-0.5,  0.5, -0.5};
    	cube->vertices[3] = (Vector3) {-0.5,  0.5,  0.5};
    	cube->vertices[4] = (Vector3) { 0.5, -0.5, -0.5};
    	cube->vertices[5] = (Vector3) { 0.5, -0.5,  0.5};
    	cube->vertices[6] = (Vector3) { 0.5,  0.5, -0.5};
    	cube->vertices[7] = (Vector3) { 0.5,  0.5,  0.5};
}

void rotateY(Cube *cube, float angle) {
	for(int i=0; i<8; i++) {
		float oldX = cube->vertices[i].x;
		cube->vertices[i].x = oldX * cos(angle) - cube->vertices[i].z * sin(angle);
		cube->vertices[i].z = oldX * sin(angle) + cube->vertices[i].z * cos(angle);
	}
}

Vector2 transform(Vector3 *v) {
	float offset_z = v->z + 2.5f;
	if (v->z == 0) return (Vector2) {0,0};
	float newX = v->x/offset_z;
	float newY = v->y/offset_z;
	return (Vector2) {newX, newY};
}

Vector2 translate(Vector2 v) {
	float newX = ((v.x + 1) / 2) * WIDTH;
	float newY = (1 - (v.y + 1) / 2) * HEIGHT;
	return (Vector2) {newX, newY};
}

void draw(SDL_Surface *surface, Cube *cube) {
	int n = 8;
	for (int i=0; i<n;i++) {
		Vector2 transformed_vector = transform(cube->vertices + i);
		Vector2 coords = translate(transformed_vector);
		SDL_Rect rect = {coords.x, coords.y, VSIZE, VSIZE};
		//printf("%f and %f\n", coords.x, coords.y);
		SDL_FillRect(surface, &rect, WHITE);
	}
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Cube Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	
	// init cube
	Cube cube;
	init(&cube);

	float t = 0;


	// main loop 
	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) running = 0;
		SDL_Rect bg = (SDL_Rect){0,0,WIDTH,HEIGHT};
		SDL_FillRect(surface, &bg, 0x00000000);
		draw(surface, &cube);
		
		rotateY(&cube, 0.05f);
		
		
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);	 // 16
		t += 1;

	}

	free(cube.vertices);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
