#include <SDL.h>
#include <iostream>
#include "util.h"
#include "renderer.h"

Renderer::Renderer(SDL_Window * window, int x, int y) {
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	xOffset = x;
	yOffset = y;
	alpha = 255;
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(renderer);
}

void Renderer::render(SDL_Texture* texture, int x, int y) {
    SDL_SetTextureAlphaMod(texture, alpha);
	int w, h;
	// Render an entire texture at position (x, y) within the game
	SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
	SDL_Rect bgRect = {x + xOffset, y + yOffset, w, h};
	SDL_RenderCopy(renderer, texture, nullptr, &bgRect);
}

void Renderer::render(SDL_Texture* texture, SDL_Rect* clip, int x, int y) {
    SDL_SetTextureAlphaMod(texture, alpha);
	// Render the subset of the texture represented by clip at position (x, y)
	SDL_Rect bgRect = {x + xOffset, y + yOffset, clip->w, clip->h};
	SDL_RenderCopy(renderer, texture, clip, &bgRect);
}

void Renderer::renderRectangle(SDL_Rect rectangle, unsigned char r, unsigned char g, unsigned char b) {
	SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
	rectangle.x = rectangle.x + xOffset;
	rectangle.y = rectangle.y + yOffset;
	SDL_RenderFillRect(renderer, &rectangle);
}

void Renderer::renderTextBox(int startX, int startY, int width, int height) {
	SDL_Rect rectangle = {startX, startY, width, height};
	renderRectangle(rectangle, 0, 0, 0);
	rectangle = {startX + 2, startY + 2, width - 4, height - 4};
	renderRectangle(rectangle, 255, 255, 255);
}

SDL_Texture * Renderer::createTextureFromSurface(SDL_Surface* surface) {
	SDL_Texture * text = SDL_CreateTextureFromSurface(renderer, surface);
	if (text == nullptr) {
		std::cout << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
	}
	return text;
}

void Renderer::clear() {
	SDL_RenderClear(renderer);
}

void Renderer::present() {
	// Cover up the areas outside the game, so that only the game area is displayed
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_Rect border = {0,0,xOffset,yOffset * 2 + Util::GAME_HEIGHT};
	SDL_RenderFillRect(renderer, &border);
	border = {0,yOffset + Util::GAME_HEIGHT,xOffset * 2 + Util::GAME_WIDTH,yOffset};
	SDL_RenderFillRect(renderer, &border);
	border = {0,0,xOffset * 2 + Util::GAME_WIDTH,yOffset};
	SDL_RenderFillRect(renderer, &border);
	border = {xOffset + Util::GAME_WIDTH,0,xOffset,yOffset * 2 + Util::GAME_HEIGHT};
	SDL_RenderFillRect(renderer, &border);
	SDL_RenderPresent(renderer);
}

void Renderer::setAlpha(int a) {
	alpha = a;
}

void Renderer::setScale(int scaleFactor) {
	SDL_RenderSetScale(renderer, scaleFactor, scaleFactor);
}

void Renderer::setXOffset(int x) {
	xOffset = x;
}

void Renderer::setYOffset(int y) {
	yOffset = y;
}

bool Renderer::isEmpty() {
	return (renderer == nullptr);
}


