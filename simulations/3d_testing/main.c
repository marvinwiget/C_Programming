#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"

#define WHITE 0xFFFFFFFF
#define BLACK 0x00000000


#define WIDTH 800
#define HEIGHT 600

typedef struct {
    float x, y, z;
} vec3;

typedef struct {
    vec3 pos, vel;
    float width, height, depth;
} cube;

typedef struct {
    vec3 pos, vel;
    float radius;
} sphere;

vec3 vec3_alloc(float x, float y, float z) {
    return (vec3) {x, y, z};
}

void vec3_add(vec3 *dst, const vec3 *src) {
    dst->x += src->x;
    dst->y += src->y;
    dst->z += src->z;
    return;
}

cube* cube_alloc(vec3 pos, vec3 vel, float width, float height, float depth) {
    cube *c = malloc(sizeof(cube));
    c->pos = pos;
    c->vel = vel;
    c->width = width;
    c->height = height;
    c->depth = depth;
    return c;
}
sphere* sphere_alloc(vec3 pos, vec3 vel, float radius) {
    sphere *s = malloc(sizeof(sphere));
    s->pos = pos;
    s->vel = vel;
    s->radius = radius;
    return s;
}

void updateCube(cube *c);
void updateSphere(sphere *s);

void drawCube(SDL_Surface *surface, cube *c);
void drawSphere(SDL_Surface *surface, sphere *b);

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("3d stuff idk", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    srand(time(NULL));

        // main loop 
    int running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }
        SDL_Rect bg = (SDL_Rect){0,0,WIDTH,HEIGHT};
        SDL_FillRect(surface, &bg, BLACK); 
                
                
        SDL_UpdateWindowSurface(window);
        SDL_Delay(16);   // 16
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
