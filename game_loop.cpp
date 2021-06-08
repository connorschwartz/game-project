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

// High-level mode to allow us to decide what to do in the main game loop
enum Mode {
	TITLE,
	OVERWORLD,
	BATTLE,
	DIALOG,
	MENU
};
int current_mode;

// The window we'll be rendering to
SDL_Window* window = nullptr;
Renderer* renderer = nullptr;

// The current area (town, overworld, etc.)
Area* area = nullptr;

// The area that the player is going to, if they stepped on a trigger
Area* newArea = nullptr;

// We have a logical game size (GAME_WIDTH x GAME_HEIGHT) and a physical size, which can be scaled up by
// an integer (scaleFactor) based on the size of the game window
int scaleFactor = 3;
int screenWidth = Util::GAME_WIDTH * scaleFactor;
int screenHeight = Util::GAME_HEIGHT * scaleFactor;

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

	area = new Area("data/town", renderer, 5, 14);

	int quit = 0;
	SDL_Event e;
	renderer->setScale(scaleFactor);
	// Main loop - run until the game ends
	while (quit == 0) {
		// Handle player input
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = 1;
			}
			// When the window size changes, update the scale factor to make the game as big as possible
			else if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				screenWidth = e.window.data1;
				screenHeight = e.window.data2;
				scaleFactor = std::min(screenWidth / Util::GAME_WIDTH, screenHeight / Util::GAME_HEIGHT);
				renderer->setScale(scaleFactor);
				// Set the x and y distances between the top left corner of the window and the top left corner
				// of the game. This allows us to center the game in the window.
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
		if (newArea == nullptr) {
			newArea = area->checkAreaTriggers();
			if (newArea != nullptr) {
				area->fadeOut();
				area->pauseMovement();
				newArea->pauseMovement();
			}
		}
		if (area->isFaded()) {
			delete area;
			area = newArea;
			area->resumeMovement();
			newArea = nullptr;
		}
		renderer->clear();
		area->render(renderer, window);
		renderer->present();
	}

	return 0;
}

