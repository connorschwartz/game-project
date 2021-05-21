#ifndef Renderer_H
#define Renderer_H

#include <SDL.h>

class Renderer {
public:
	Renderer(SDL_Window * window, int x, int y);
	~Renderer();

	void render(SDL_Texture* texture, int x, int y);					// Render full sprite
	void render(SDL_Texture* texture, SDL_Rect* clip, int x, int y);	// Render part of the sprite

	SDL_Texture * createTextureFromSurface(SDL_Surface* surface);

	void clear();
	void present();

	// Update the scaling between the game resolution and the window resolution
	void setScale(int scaleFactor);

	// Set offset values
	void setXOffset(int x);
	void setYOffset(int y);

	// Indicates if the renderer has not been successfully initialized
	bool isEmpty();

private:
	SDL_Renderer * renderer;
	int xOffset;		// Horizontal distance from the edge of the window to the edge of the game
	int yOffset;		// Vertical distance from the edge of the window to the edge of the game
};


#endif

