#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include <SDL2/SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define PARTICLE_SIZE 3
#define MAX_SPEED 30
#define N_PARTICLES 1200

#define GRAVITY 0.0
#define USER_GRAV 0.7
#define DAMPENING 0.9
#define COLL_DAMPENING 0.6

#define WHITE 0xFFFFFFFF

typedef struct { float x, y; } Vec2;
typedef struct {
        Vec2 *pos;
        Vec2 *vel;
        float radius;
        uint32_t color;
} Particle;

// global vars
Particle **PARTICLES;
int particles_initialized = 0;
Particle *user_particle;
bool USER_GRAV_ACTIVE = false;

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
void particleCollision(Particle *p, Particle *other) {
        Vec2 *pos = p->pos;
        Vec2 *vel = p->vel;
        Vec2 *o_pos = other->pos;
        Vec2 *o_vel = other->vel;
        float distance = sqrt(pow(pos->x - o_pos->x, 2) +
                              pow(pos->y - o_pos->y, 2));
        if (p->radius + other->radius > distance) {
                if (distance == 0) {
                        pos->x += 0.1f;
                        distance = 0.1f;
                }
                float overlap = (p->radius + other->radius - distance);
                // Use a small 'damping' factor (0.5 means each moves half the overlap)
                float force = 0.5f; 

                // 1. Calculate the direction (Normal)
                float nx = (pos->x - o_pos->x) / distance;
                float ny = (pos->y - o_pos->y) / distance;

                // 2. Push along the normal
                pos->x += nx * overlap * force;
                pos->y += ny * overlap * force;

                o_pos->x -= nx * overlap * force;
                o_pos->y -= ny * overlap * force;

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

                // enforce max speed
                p->vel->x = (p->vel->x > 0) ? fmin(MAX_SPEED, p->vel->x) : fmax(-MAX_SPEED, p->vel->x);
                p->vel->y = (p->vel->y > 0) ? fmin(MAX_SPEED, p->vel->y) : fmax(-MAX_SPEED, p->vel->y);

                other->vel->x = (other->vel->x > 0) ? 
                                fmin(MAX_SPEED, other->vel->x) : fmax(-MAX_SPEED,other->vel->x);
                other->vel->y = (other->vel->y > 0) ?
                                fmin(MAX_SPEED, other->vel->y) : fmax(-MAX_SPEED,other->vel->y);
                p->vel->x *= COLL_DAMPENING;
                p->vel->y *= COLL_DAMPENING;
                other->vel->x *= COLL_DAMPENING;
                other->vel->y *= COLL_DAMPENING;
        }
}

void updateParticle(Particle *p) {
        Vec2 *pos = p->pos;
        Vec2 *vel = p->vel;


        // GRAVITY AND DAMPENING
        vel->y += GRAVITY;
        vel->x *= DAMPENING;
        vel->y *= DAMPENING;

        // USER GRAVITY
        if (USER_GRAV_ACTIVE) {
                float gx = user_particle->pos->x;
                float gy = user_particle->pos->y;

                vel->x += (gx > pos->x) ? USER_GRAV : -USER_GRAV;
                vel->y += (gy > pos->y) ? USER_GRAV : -USER_GRAV;
        }       

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

int isColliding(int x, int y) {
        for (int i=0;i<particles_initialized;i++) {
                Particle *other = PARTICLES[i];
                float distance = sqrt(pow(x-other->pos->x, 2) + pow(y-other->pos->y, 2));
                if (distance < PARTICLE_SIZE*2) return 1;
        }
        return 0;
}

Particle* initParticle() {
        Particle *p = malloc(sizeof(Particle));

        // random start pos
        int x_pos;
        int y_pos;
        int attemps = 0;
        do {
                int rand_x = rand() % WIDTH;
                x_pos = (rand_x < PARTICLE_SIZE) ? PARTICLE_SIZE :
                        (rand_x > WIDTH - PARTICLE_SIZE) ? WIDTH - PARTICLE_SIZE : rand_x;

                int rand_y = rand() % HEIGHT;
                y_pos = (rand_y < PARTICLE_SIZE) ? PARTICLE_SIZE : 
                        (rand_y > HEIGHT - PARTICLE_SIZE) ? HEIGHT - PARTICLE_SIZE : rand_y;
                attemps++;
                if (attemps > 1000) printf("no space for particles"); break;
        } while (isColliding(x_pos, y_pos));
        p->pos = initVector(x_pos, y_pos);
        p->vel = initVector(rand() % MAX_SPEED, rand() & MAX_SPEED);
        p->radius = PARTICLE_SIZE;
        p->color = WHITE;
        return p;
}

Particle** initParticles() {
        PARTICLES = malloc(sizeof(Particle*) * N_PARTICLES);
        for (int i=0; i<N_PARTICLES; i++) {
                PARTICLES[i] = initParticle();
                particles_initialized++;
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
                for (int j=0;j<N_PARTICLES;j++) {
                        if (j > i) particleCollision(p[i], p[j]);
                }
                updateParticle(p[i]);
                drawParticle(surface, p[i]);
        }
}
int main() {
        SDL_Init(SDL_INIT_VIDEO);
        SDL_Window *window = SDL_CreateWindow("Particle Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
        SDL_Surface *surface = SDL_GetWindowSurface(window);
        srand(time(NULL));

        PARTICLES = initParticles();
        // init gravity particle
        user_particle = initParticle();
        user_particle->pos = initVector(0,0);
        user_particle->vel = initVector(0,0);
        user_particle->radius = 30;
        user_particle->color = 0xABCDEFAB;

        // main loop 
        int running = 1;
        SDL_Event event;

        while (running) {
                while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_QUIT) running = 0;
                        if (event.type == SDL_MOUSEBUTTONDOWN) {
                                if (event.button.button == SDL_BUTTON_LEFT) {
                                        user_particle->pos->x = event.button.x;
                                        user_particle->pos->y = event.button.y;
                                        USER_GRAV_ACTIVE = true;
                                }
                        }
                        if (event.type == SDL_MOUSEBUTTONUP) {
                                if (event.button.button == SDL_BUTTON_RIGHT) {
                                        USER_GRAV_ACTIVE = false;
                                }
                        }
                        
                }
                SDL_Rect bg = (SDL_Rect){0,0,WIDTH,HEIGHT};
                SDL_FillRect(surface, &bg, 0x00000000); 

                ALL_PARTICLES(surface, PARTICLES);
                if (USER_GRAV_ACTIVE) drawParticle(surface, user_particle);
                
                SDL_UpdateWindowSurface(window);
                SDL_Delay(16);   // 16
        }

        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
}
