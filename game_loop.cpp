//Using SDL and standard IO
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include "util.h"
#include "area.h"
#include "renderer.h"

enum Mode {
	TITLE,
	OVERWORLD,
	BATTLE,
	MENU
};


//The window we'll be rendering to
SDL_Window* window = nullptr;
Renderer* renderer = nullptr;

Area* area = nullptr;

int scaleFactor = 3;
int screenWidth = Util::GAME_WIDTH * scaleFactor;
int screenHeight = Util::GAME_HEIGHT * scaleFactor;

int current_mode;

int init() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize with error " << SDL_GetError() << std::endl;
		return 1;
	}
	else {
		// Create the window
		window = SDL_CreateWindow("Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window == nullptr) {
			std::cout << "Window could not be created with SDL Error: " << SDL_GetError() << std::endl;
			return 1;
		}
		else {
			// Initialize the renderer
			renderer = new Renderer(window, (screenWidth - Util::GAME_WIDTH * scaleFactor) / (2 * scaleFactor), (screenHeight - Util::GAME_HEIGHT * scaleFactor) / (2 * scaleFactor));
			if (renderer->isEmpty()) {
				std::cout << "Renderer could not be created with SDL Error: " << SDL_GetError() << std::endl;
				return 1;
			}
			else {
				// Initialize SDL_Image to load PNGs
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags)) & imgFlags) {
					std::cout << "SDL_image could not be created with SDL Error: " << IMG_GetError() << std::endl;
					return 1;
				}
				// Initialize SDL_ttf for text display
				if (TTF_Init() == -1)
				{
					std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
					return 1;
				}
				// Initialize SDL_mixer for sound/music
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
					std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
					return 1;
				}
			}
		}
	}
	current_mode = OVERWORLD;
	return 0;
}

int close() {

    SDL_DestroyWindow(window);

    delete renderer;
    delete area;

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	return 0;
}

int main( int argc, char* args[] )
{
	if(init() != 0) {
		std::cout << "Failed to start game" << std::endl;
		close();
		return 1;
	}

	area = new Area("asdf", renderer);

	int quit = 0;
	SDL_Event e;
	renderer->setScale(scaleFactor);
	while (quit == 0) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				screenWidth = e.window.data1;
				screenHeight = e.window.data2;
				scaleFactor = std::min(screenWidth / Util::GAME_WIDTH, screenHeight / Util::GAME_HEIGHT);
				renderer->setScale(scaleFactor);
				renderer->setXOffset((screenWidth - scaleFactor * Util::GAME_WIDTH) / (2 * scaleFactor));
				renderer->setYOffset((screenHeight - scaleFactor * Util::GAME_HEIGHT) / (2 * scaleFactor));
			}
			else {
				area->handleInput(e);
			}
		}
		const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
		area->handleKeyStates(currentKeyStates);
		area->moveObjects();
		renderer->clear();
		area->render(renderer);
		renderer->present();
	}

	return 0;
}

