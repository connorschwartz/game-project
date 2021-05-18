#ifndef Renderer_H
#define Renderer_H

#include <SDL.h>

class Renderer {
public:
	Renderer(SDL_Window * window, int x, int y);
	~Renderer();

	void render(SDL_Texture* texture, int x, int y);
	void render(SDL_Texture* texture, SDL_Rect* clip, int x, int y);

	SDL_Texture * createTextureFromSurface(SDL_Surface* surface);

	void clear();
	void present();

	void setScale(int scaleFactor);

	void setXOffset(int x);
	void setYOffset(int y);

	bool isEmpty();

private:
	SDL_Renderer * renderer;
	int xOffset;
	int yOffset;
};


#endif

