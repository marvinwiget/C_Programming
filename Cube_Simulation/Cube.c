// gcc -o Cube Cube.c `sdl2-config --cflags --libs`

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define WIDTH 800
#define HEIGHT 600

#define WHITE 0xffffffff


int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *window = SDL_CreateWindow("Cube Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	
	int running = 1;
	SDL_Event event;
	while (running) {
		while (SDL_PollEvent(&event)) if (event.type == SDL_QUIT) running = 0;
		

		SDL_UpdateWindowSurface(window);
		SDL_Delay(16);	
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
