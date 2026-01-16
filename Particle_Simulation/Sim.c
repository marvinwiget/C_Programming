#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SDL.h"

#define WIDTH 800
#define HEIGHT 600

#define PARTICLE_SIZE 30
#define N_PARTICLES 10

#define GRAVITY 10

#define WHITE 0xFFFFFFFF

typedef struct { float x, y; } Vec2;
typedef struct {
	Vec2 *pos;
	Vec2 *vel;
	float radius;
	uint32_t color;
} Particle;

Vec2* initVector(float x, float y) {
	Vec2 *v = malloc(sizeof(Vec2));
	v->x = x;
	v->y = y;
	return v;
}

void updateParticle(Particle *p) {
	
}

Particle* initParticle() {
	Particle *p = malloc(sizeof(Particle));
	int rand_x = rand() % WIDTH;
	int x_pos = (rand_x < PARTICLE_SIZE) ? PARTICLE_SIZE :
	     	    (rand_x > WIDTH - PARTICLE_SIZE) ? WIDTH - PARTICLE_SIZE : rand_x;
	int rand_y = rand() % HEIGHT;
	int y_pos = (rand_y < PARTICLE_SIZE) ? PARTICLE_SIZE : 
		    (rand_y > HEIGHT - PARTICLE_SIZE) ? HEIGHT - PARTICLE_SIZE : rand_y;
	p->pos = initVector(x_pos, y_pos);
	p->vel = initVector(0, 0);
	p->radius = PARTICLE_SIZE;
	p->color = WHITE;
	return p;
}

void drawParticle(SDL_Surface *surface, Particle *p) {
	float x = p->pos->x;
	float y = p->pos->y;	
	float r = p->radius;
	uint32_t color = p->color;
	for (int i = x - r; i < x + r; i++ ) {
		for (int j = y - r; j < y + r; j++) {
			if (r < sqrt(pow((x - i), 2) + pow((y - j), 2))) continue;
			SDL_Rect rect = (SDL_Rect) {i, j, 1, 1};
			SDL_FillRect(surface, &rect, color);
		}
	}
}

int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Cube Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	
	Particle *p = initParticle();
	
	// main loop 
	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) running = 0;
		SDL_Rect bg = (SDL_Rect){0,0,WIDTH,HEIGHT};
		SDL_FillRect(surface, &bg, 0x00000000);	

		drawParticle(surface, p);	
		
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);	 // 16
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

