#include <string>
#include <iostream>
#include <SDL.h>
#include <fstream>
#include "util.h"
#include "area.h"
#include "player.h"

void Area::initializeArea(std::string fileName) {
	std::ifstream data;
	data.open(fileName, std::ios_base::out);
	std::string text1;
	std::string text2;
	data >> text1;
	data >> text2;
	testText = Util::loadTextureFromText(text1.c_str(), renderer, 15);
	testText2 = Util::loadTextureFromText(text2.c_str(), renderer, 15);
	data >> xBlocks;
	data >> yBlocks;
	xPixels = xBlocks * Util::BLOCK_SIZE;
	yPixels = yBlocks * Util::BLOCK_SIZE;
	areaBlocks = new AreaBlocks(xBlocks, yBlocks);
	data.close();
}

void Area::initializeBackground(std::string fileName) {
	std::ifstream data;
	data.open(fileName, std::ios_base::out);
	std::string backgroundImage;
	data >> backgroundImage;
	grassTextures = Util::loadTexture(backgroundImage, renderer);
	grassTiles[0] = {0,16,16,16};
	grassTiles[1] = {16,16,16,16};
	grassTiles[2] = {32,16,16,16};
	grassTiles[3] = {48,16,16,16};
	grassMap = new int * [xBlocks];
	for (int i = 0; i < xBlocks; i++) {
		grassMap[i] = new int[yBlocks];
		for (int j = 0; j < yBlocks; j++) {
			int tileType = -1;
			data >> grassMap[i][j];
		}
	}
	data.close();
}

void Area::initializeStillObjects(std::string fileName) {
	std::ifstream data;
	data.open(fileName, std::ios_base::out);
	int index;
	data >> index;
	std::string treeFilename;
	data >> treeFilename;
	SDL_Texture * treeSprite = Util::loadTexture(treeFilename, renderer);
	int spriteNum;
	data >> spriteNum;
	int xPos, yPos;
	data >> xPos;
	data >> yPos;
	int blockWidth, blockHeight;
	data >> blockWidth;
	data >> blockHeight;
	int width, height;
	data >> width;
	data >> height;
	tree = new StillObject(treeSprite, areaBlocks, xPos, yPos, xBlocks, yBlocks, blockWidth, blockHeight, width, height);
	data.close();
}

void Area::initializePlayer(std::string fileName) {
	std::ifstream data;
	data.open(fileName, std::ios_base::out);
	std::string playerFilename;
	data >> playerFilename;
	SDL_Texture * playerSprite = Util::loadTexture(playerFilename, renderer);
	int xPos, yPos;
	data >> xPos;
	data >> yPos;
	int width, height;
	data >> width;
	data >> height;
	player = new Player(playerSprite, areaBlocks, xPos, yPos, xBlocks, yBlocks, width, height);
	data.close();
}

Area::Area(std::string directory, Renderer * r) {
	renderer = r;
	initializeArea(directory + "/area.txt");
	initializeBackground(directory + "/background.txt");
	initializeStillObjects(directory + "/still_objects.txt");
	initializePlayer(directory + "/player.txt");
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
	tree->render(renderer, cameraX, cameraY);
}




