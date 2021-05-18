#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>
#include "util.h"

SDL_Texture* Util::loadTexture(std::string fname, Renderer *renderer) {
	SDL_Texture* newTexture = nullptr;

	SDL_Surface* startSurf = IMG_Load(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}

	newTexture = renderer->createTextureFromSurface(startSurf);
	if (newTexture == nullptr) {
		std::cout << "Unable to create texture from " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return newTexture;
}

SDL_Texture* Util::loadTextureFromText(std::string text, Renderer *renderer, std::string fontName, int fontSize, SDL_Color color) {
	TTF_Font* font = TTF_OpenFont(fontName.c_str(), fontSize);
	// Render the surface from the file
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
	SDL_Texture* texture = nullptr;
	TTF_CloseFont(font);
	if (textSurface == nullptr) {
		std::cout << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
	}
	else {
		texture = renderer->createTextureFromSurface(textSurface);
	}
	SDL_FreeSurface(textSurface);
	return texture;
}

SDL_Texture* Util::loadTextureFromText(std::string text, Renderer *renderer, int fontSize) {
	SDL_Texture* texture = loadTextureFromText(text, renderer, "fonts/AtlantisInternational.ttf", fontSize, {0, 0, 0});
	return texture;
}

Mix_Music* Util::loadMusic(std::string fileName) {
	Mix_Music* music = Mix_LoadMUS(fileName.c_str());
	if (music == nullptr) {
		std::cout << "Failed to load music! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return music;
}

Mix_Chunk* Util::loadSound(std::string fileName) {
	Mix_Chunk* sound = Mix_LoadWAV(fileName.c_str());
	if (sound == nullptr) {
		std::cout << "Failed to load sound! SDL_mixer Error: " << Mix_GetError() << std::endl;
	}
	return sound;
}
