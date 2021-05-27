#include <SDL.h>
#include <string>
#include <iostream>
#include "renderer.h"

class Background {
public:
	Background(std::ifstream& data, Renderer * renderer, int areaWidth, int areaHeight, int sheetWidth, int sheetHeight);
	~Background();

	void render(Renderer * renderer, int cameraX, int cameraY);

private:

	SDL_Texture * grassTextures = nullptr;				// Grass sprite sheet
	SDL_Rect * grassTiles;								// Area of each grass sprite on the sheet
	int ** grassMap;									// grass sprite number for each block
};



