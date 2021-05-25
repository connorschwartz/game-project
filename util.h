#ifndef Util_H
#define Util_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <string>
#include "renderer.h"

class Util {
public:
	static SDL_Texture* loadTexture(std::string fname, Renderer *renderer);

	static SDL_Texture* loadTextureFromText(std::string text, Renderer * renderer, TTF_Font * font, SDL_Color color);

	static SDL_Texture* loadTextureFromText(std::string text, Renderer *renderer, std::string fontName, int fontSize, SDL_Color color);

	static SDL_Texture* loadTextureFromText(std::string text, Renderer *renderer, int fontSize);

	static Mix_Music* loadMusic(std::string fileName);

	static Mix_Chunk* loadSound(std::string fileName);

	//Screen dimension constants
	static const int GAME_WIDTH = 256;
	static const int GAME_HEIGHT = 224;

	// Width and height of each block, in pixels
	static const int BLOCK_SIZE = 16;
};

#endif

