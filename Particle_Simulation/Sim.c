#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "SDL.h"

#define WIDTH 800
#define HEIGHT 600

#define PARTICLE_SIZE 10
#define MAX_SPEED 1
#define N_PARTICLES 600

#define GRAVITY 0.1
#define DAMPENING 0.99

#define WHITE 0xFFFFFFFF

typedef struct { float x, y; } Vec2;
typedef struct {
	Vec2 *pos;
	Vec2 *vel;
	float radius;
	uint32_t color;
} Particle;

Particle **PARTICLES;

Vec2 scaleVec2(float scalar, Vec2 *v) {
	Vec2 res;	
	res.x = v->x * scalar;
	res.y = v->y * scalar;
	return res;
}

Vec2 minusVec2(Vec2 *a, Vec2 *b) {
	Vec2 res;
	res.x = a->x - b->x;
	res.y = a->y - b->y;
	return res;
}

float dotProduct(Vec2 *a, Vec2 *b) {
	return a->x * b->x + a->y * b->y;
}

Vec2* initVector(float x, float y) {
	Vec2 *v = malloc(sizeof(Vec2));
	v->x = x;
	v->y = y;
	return v;
}
void particleCollision(Particle *p) {
	Vec2 *pos = p->pos;
	Vec2 *vel = p->vel;
	


	for (int i=0; i<N_PARTICLES;i++) {
		if (PARTICLES[i] == p) continue;
		Particle *other = PARTICLES[i];
		Vec2 *o_pos = other->pos;
		Vec2 *o_vel = other->vel;

		float distance = sqrt(pow(pos->x - o_pos->x, 2) +
				      pow(pos->y - o_pos->y, 2));
		if (sqrt(pow(distance,2)) < 0.0001f) distance = 0.0001f;
		if (p->radius + other->radius > distance) {

			Vec2 minus1 = minusVec2(vel, o_vel);
			Vec2 minus2 = minusVec2(o_vel, vel);
			Vec2 minus3 = minusVec2(pos, o_pos);
			Vec2 minus4 = minusVec2(o_pos, pos);
			float dot1 = dotProduct(&minus1, &minus3);
			float dot2 = dotProduct(&minus2, &minus4);
			Vec2 scale1 = scaleVec2(dot1/pow(distance, 2), &minus3);
			Vec2 scale2 = scaleVec2(dot2/pow(distance, 2), &minus4);

			*p->vel = minusVec2(vel, &scale1);

			*other->vel = minusVec2(o_vel, &scale2);

		}
	}
}

void updateParticle(Particle *p) {
	Vec2 *pos = p->pos;
	Vec2 *vel = p->vel;


	// GRAVITY AND DAMPENING
	vel->y += (pos->y < HEIGHT - p->radius) ? GRAVITY : 0;
	vel->x *= DAMPENING;
	vel->y *= DAMPENING;
	
	// Check particle collisions
	particleCollision(p);


	// SET NEW POS
	pos->x += vel->x;
	pos->y += vel->y;

	// WALL COLLISIONS
	if (pos->x - p->radius < 0) {
		vel->x = -vel->x*0.8; // invert x velocity
		pos->x = p->radius;
	}
	if (pos->x + p->radius > WIDTH) {
		vel->x = -vel->x*0.8;
		pos->x = WIDTH - p->radius;
	}
	if (pos->y - p->radius < 0) {
		vel->y = -vel->y*0.8; // invert y velocity
		pos->y = p->radius;
	}
	if (pos->y + p->radius > HEIGHT) {
		vel->y = -vel->y*0.8;
		pos->y = HEIGHT - p->radius;
	}

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
	p->vel = initVector(rand() % MAX_SPEED, rand() & MAX_SPEED);
	p->radius = PARTICLE_SIZE;
	p->color = WHITE;
	return p;
}

Particle** initParticles() {
	Particle **PARTICLES = malloc(sizeof(Particle*) * N_PARTICLES);
	for (int i=0; i<N_PARTICLES; i++) {
		PARTICLES[i] = initParticle();
	}
	return PARTICLES;
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

void ALL_PARTICLES(SDL_Surface *surface, Particle **p) {
	for (int i=0;i<N_PARTICLES;i++) {
		updateParticle(p[i]);
		drawParticle(surface, p[i]);
	}
}
int main(int argc, char *argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	srand(time(NULL));
	PARTICLES = initParticles();

	// main loop 
	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) running = 0;
		SDL_Rect bg = (SDL_Rect){0,0,WIDTH,HEIGHT};
		SDL_FillRect(surface, &bg, 0x00000000);	

		ALL_PARTICLES(surface, PARTICLES);
		
		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);	 // 16
	}

	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

