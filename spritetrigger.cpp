#include "spritetrigger.h"
#include <iostream>

SpriteTrigger::SpriteTrigger(int xBlock, int yBlock, int blockWidth, int blockHeight, int xPixel, int yPixel, int pixelWidth, int pixelHeight) : xBlock{xBlock}, yBlock{yBlock}, blockWidth{blockWidth}, blockHeight{blockHeight}, xPixel{xPixel}, yPixel{yPixel}, pixelWidth{pixelWidth}, pixelHeight{pixelHeight} {
	triggered = false;
}

SpriteTrigger::~SpriteTrigger() {

}

bool SpriteTrigger::isTriggered() {
	return triggered;
}

void SpriteTrigger::update(Player * player) {
	// If the player is touching the trigger or moving towards the trigger, it's hit
	int playerX = player->getBlockX();
	int playerY = player->getBlockY();
	int playerIntendedX = player->getIntendedXBlock();
	int playerIntendedY = player->getIntendedYBlock();
	if ((playerX >= xBlock && playerY >= yBlock && playerX < xBlock + blockWidth && playerY < yBlock + blockHeight) ||
			(playerIntendedX >= xBlock && playerIntendedY >= yBlock && playerIntendedX < xBlock + blockWidth && playerIntendedY < yBlock + blockHeight)) {
		triggered = true;
	}
	else {
		triggered = false;
	}
}

SDL_Rect SpriteTrigger::getRegion(int cameraX, int cameraY, int objectX, int objectY) {
	return {xPixel - cameraX, yPixel - cameraY, pixelWidth, pixelHeight};
}
