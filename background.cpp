#include <fstream>
#include "background.h"
#include "util.h"


Background::Background(std::ifstream& data, Renderer * renderer, int areaWidth, int areaHeight, int sheetWidth, int sheetHeight) {
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
	for (int i = cameraX / Util::BLOCK_SIZE; i <= (cameraX + Util::GAME_WIDTH - 1) / Util::BLOCK_SIZE; i++) {
		for (int j = cameraY / Util::BLOCK_SIZE; j <= (cameraY + Util::GAME_HEIGHT - 1) / Util::BLOCK_SIZE; j++) {
			renderer->render(grassTextures, &grassTiles[grassMap[j][i]], i * Util::BLOCK_SIZE - cameraX, j * Util::BLOCK_SIZE - cameraY);
		}
	}
}


