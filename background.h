#include <SDL.h>
#include <string>
#include <iostream>
#include "renderer.h"

class Background {
public:
	Background(std::ifstream& data, Renderer * renderer, int width, int height, int sheetWidth, int sheetHeight);
	~Background();

	void render(Renderer * renderer, int cameraX, int cameraY);

	int getTileType(int x, int y);		// Returns the tile number of background tile (x, y)

private:

	SDL_Texture * grassTextures = nullptr;				// Grass sprite sheet
	SDL_Rect * grassTiles;								// Area of each grass sprite on the sheet
	int ** grassMap;									// grass sprite number for each block
	int areaWidth, areaHeight;							// Dimensions of the area
};




