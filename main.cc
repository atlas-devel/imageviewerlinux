#include <iostream>
#include <fstream>
//#include <Windows.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include <winuser.h>
#include <math.h>  
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <SDL2/SDL_syswm.h>
#define ERROR_NOT_ENOUGH_ARGS 1
#define ERROR_INIT 2
#define ERROR_IMAGE_OPEN 3
#define ERROR_RESOLUTION_TOO_BIG 4

int main(int argc, char* argv[]) {
	
	if(argc < 2) {
		std::cout << "Not enough arguments. usage: image [path to file]\n";
		return ERROR_NOT_ENOUGH_ARGS;
	}
	Display* d = XOpenDisplay(NULL);
	Screen* sc = DefaultScreenOfDisplay(d);
	int screenx = sc->width;
	int screeny = sc->height;
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

		
	
	
	bool quit = false;
	//int screenx = GetSystemMetrics(SM_CXFULLSCREEN);
	// int screeny = GetSystemMetrics(SM_CYFULLSCREEN);
	int newx = -1;
	int newy = -1;
	int downscaled = 0;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = NULL;
	SDL_Texture* imageTex = NULL;
	SDL_SetTextureScaleMode(imageTex, SDL_ScaleModeBest);
	SDL_Surface* imageSurface = NULL;
	imageSurface = IMG_Load(argv[1]);
	if (imageSurface->w > screenx || imageSurface->h > screeny) {
		newx = imageSurface->w;
		newy = imageSurface->h;
		while (newx > screenx || newy > screeny) {
			if (downscaled < 30) {
				newx = round(newx / 2);
				newy = round(newy / 2);
				downscaled++;
				std::cout << "Resolution too big. Downscaling." << std::endl;
			}
			else {
				break;
			}
		
		}
		if (downscaled >= 30) {
			std::cout << "Resolution way too big. Quitting." << std::endl;
			return ERROR_RESOLUTION_TOO_BIG;
		}

		window = SDL_CreateWindow("Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, newx, newy, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}
	else {


		window = SDL_CreateWindow("Image", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, imageSurface->w, imageSurface->h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	}
	
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);




	imageTex = SDL_CreateTextureFromSurface(renderer, imageSurface);
	SDL_FreeSurface(imageSurface);
	SDL_RenderCopy(renderer, imageTex, NULL, NULL);
	SDL_RenderPresent(renderer);
	while (!quit) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_RESIZED || SDL_WINDOWEVENT_SIZE_CHANGED) {
					SDL_RenderClear(renderer);
					SDL_RenderCopy(renderer, imageTex, NULL, NULL);
					SDL_RenderPresent(renderer);
					
				}
			}
			
			
			
		}
		
	}
	SDL_DestroyTexture(imageTex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
