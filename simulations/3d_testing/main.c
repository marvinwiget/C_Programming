#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "SDL2/SDL.h"
#include "util.h"
#include "render.h"

#define WHITE 0xFFFFFF
#define BLACK 0x000000
#define BG_COLOR 0x0000FFFF

#define WIDTH 800
#define HEIGHT 600
#define FOV 500.0f
#define INF 9e9

uint32_t framebuffer[WIDTH * HEIGHT];
float z_buffer[WIDTH * HEIGHT];

void put_pixel(int x, int y, float z, uint32_t color) {
    int index = y * WIDTH + x;
    if (z < z_buffer[index]) {
        framebuffer[index] = color;
        z_buffer[index] = z;
    }
    return;
}

void clear_buffers(void) {
    for (int i=0; i < WIDTH*HEIGHT;i++) {
        framebuffer[i] = BG_COLOR;
        z_buffer[i] = INF;
    }
    return;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("3d stuff idk", SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    srand(time(NULL));
    int running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }
        clear_buffers();

        // stuff



        SDL_UpdateTexture(texture, NULL, framebuffer, WIDTH * sizeof(uint32_t));

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);   // 16
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
