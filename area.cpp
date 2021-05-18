#include <string>
#include <iostream>
#include <SDL.h>
#include <fstream>
#include "util.h"
#include "area.h"
#include "player.h"

Area::Area(std::string fileName, Renderer* renderer) {
	grassTextures = Util::loadTexture("images/grass.png", renderer);
	testText = Util::loadTextureFromText("qwertyuiopasdfghjklzxcvbnm  1234567890", renderer, 15);
	testText2 = Util::loadTextureFromText("QWERTYUIOPASDFGHJKLZXCVBNM", renderer, 15);
	grassTiles[0] = {0,16,16,16};
	grassTiles[1] = {16,16,16,16};
	grassTiles[2] = {32,16,16,16};
	grassTiles[3] = {48,16,16,16};
	xBlocks = 32;
	yBlocks = 64;
	xPixels = xBlocks * Util::BLOCK_SIZE;
	yPixels = yBlocks * Util::BLOCK_SIZE;
	std::ifstream map("data/grass_mapping.txt");
	grassMap = new int * [xBlocks];
	for (int i = 0; i < xBlocks; i++) {
		grassMap[i] = new int[yBlocks];
		for (int j = 0; j < yBlocks; j++) {
			int tileType = -1;
			map >> grassMap[i][j];
		}
	}
	areaBlocks = new AreaBlocks(xBlocks, yBlocks);
	player = new Player(renderer, areaBlocks, 5, 5, xBlocks, yBlocks);
	Mix_PlayMusic(testMusic, -1);
}

Area::~Area() {
	SDL_DestroyTexture(grassTextures);
	SDL_DestroyTexture(testText);
	SDL_DestroyTexture(testText2);
    delete player;
	delete grassMap;
}

void Area::handleInput(SDL_Event e) {
	if (e.key.keysym.sym == SDLK_1) {
		Mix_PlayChannel( -1, testSound, 0 );
	}
}

void Area::handleKeyStates(const Uint8* currentKeyStates) {
	player->handleKeyStates(currentKeyStates);
}

void Area::moveObjects() {
	player->move();
}

void Area::render(Renderer* renderer) {
	int playerX = player->getCenterX();
	int playerY = player->getCenterY();
	int cameraX = playerX - Util::GAME_WIDTH / 2;
	int cameraY = playerY - Util::GAME_HEIGHT / 2;
	if (cameraX < 0) cameraX = 0;
	if (cameraY < 0) cameraY = 0;
	if (cameraX + Util::GAME_WIDTH > xPixels) cameraX = xPixels - Util::GAME_WIDTH;
	if (cameraY + Util::GAME_HEIGHT > yPixels) cameraY = yPixels - Util::GAME_HEIGHT;
	SDL_Rect bgClip = {cameraX, cameraY, Util::GAME_WIDTH, Util::GAME_HEIGHT};
	for (int i = cameraX / Util::BLOCK_SIZE; i <= (cameraX + Util::GAME_WIDTH - 1) / Util::BLOCK_SIZE; i++) {
		for (int j = cameraY / Util::BLOCK_SIZE; j <= (cameraY + Util::GAME_HEIGHT - 1) / Util::BLOCK_SIZE; j++) {
			renderer->render(grassTextures, &grassTiles[grassMap[i][j]], i * Util::BLOCK_SIZE - cameraX, j * Util::BLOCK_SIZE - cameraY);
		}
	}
	renderer->render(testText, 30, 200);
	renderer->render(testText2, 30, 210);
	player->render(renderer, cameraX, cameraY);
}




