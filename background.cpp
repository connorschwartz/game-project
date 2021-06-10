#include <fstream>
#include "background.h"
#include "util.h"


Background::Background(std::ifstream& data, Renderer * renderer, int width, int height, int sheetWidth, int sheetHeight) : areaWidth{width}, areaHeight{height} {
	using namespace std;
	string backgroundImage;
	data >> backgroundImage;
	grassTextures = Util::loadTexture(backgroundImage, renderer);
	// Get the rectangles representing each grass block from the texture sheet
	grassTiles = new SDL_Rect[sheetWidth * sheetHeight];
	for (int i = 0; i < sheetHeight; i++) {
		for (int j = 0; j < sheetWidth; j++) {
			grassTiles[i * sheetWidth + j] = {j * Util::BLOCK_SIZE, i * Util::BLOCK_SIZE, Util::BLOCK_SIZE, Util::BLOCK_SIZE};
		}
	}
	// Get the block texture number for each block
	grassMap = new int * [areaHeight];
	for (int i = 0; i < areaHeight; i++) {
		grassMap[i] = new int[areaWidth];
		for (int j = 0; j < areaWidth; j++) {
			data >> grassMap[i][j];
		}
	}
}

Background::~Background() {
	SDL_DestroyTexture(grassTextures);
	delete grassMap;
	delete grassTiles;
}

void Background::render(Renderer * renderer, int cameraX, int cameraY) {
	int startY = cameraY / Util::BLOCK_SIZE;
	int endY = (cameraY + Util::GAME_HEIGHT - 1) / Util::BLOCK_SIZE;
	// Render the entire background lengthwise if we're given a negative camera value, as it means the area is horizontally small
	if (cameraY < 0) {
		startY = 0;
		endY = areaHeight - 1;
	}
	int startX = cameraX / Util::BLOCK_SIZE;
	int endX = (cameraX + Util::GAME_WIDTH - 1) / Util::BLOCK_SIZE;
	// Render the entire background heightwise if we're given a negative camera value, as it means the area is vertically small
	if (cameraX < 0) {
		startX = 0;
		endX = areaWidth - 1;
	}
	for (int i = startX; i <= endX; i++) {
		for (int j = startY; j <= endY; j++) {
			renderer->render(grassTextures, &grassTiles[grassMap[j][i]], i * Util::BLOCK_SIZE - cameraX, j * Util::BLOCK_SIZE - cameraY);
		}
	}
}


