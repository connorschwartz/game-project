#include <iostream>
#include "util.h"
#include "player.h"


Player::Player(SDL_Texture * s, AreaBlocks* blocks, int x, int y, std::string spriteDirection, int width, int height) : Person(blocks, x, y, spriteDirection, width, height) {
	sprites = s;
	// Create rectangles to represent each sprite on the player's sprite sheet
	for (int i = 0; i < SHEET_WIDTH; i++) {
		for (int j = 0; j < SHEET_HEIGHT; j++) {
			spriteSheet[i][j] = {i * SPRITE_WIDTH, j * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
		}
	}
	areaBlocks = blocks;
	// Mark the player's starting position as used
	areaBlocks->markUsed(xBlock, yBlock);
	upPressedTime = -1;
	leftPressedTime = -1;
	rightPressedTime = -1;
	downPressedTime = -1;
}

Player::~Player() {
	SDL_DestroyTexture(sprites);
}

void Player::handleKeyStates(const Uint8* currentKeyStates) {

	// Determine the direction in which the player wants to move
	desiredSpeed = 0;
	if (currentKeyStates[SDL_SCANCODE_W] && upPressedTime < 0) {
		upPressedTime = SDL_GetTicks();
	}
	else if (!currentKeyStates[SDL_SCANCODE_W]) {
		upPressedTime = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_S] && downPressedTime < 0) {
		downPressedTime = SDL_GetTicks();
	}
	else if (!currentKeyStates[SDL_SCANCODE_S]) {
		downPressedTime = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_A] && leftPressedTime < 0) {
		leftPressedTime = SDL_GetTicks();
	}
	else if (!currentKeyStates[SDL_SCANCODE_A]) {
		leftPressedTime = -1;
	}
	if (currentKeyStates[SDL_SCANCODE_D] && rightPressedTime < 0) {
		rightPressedTime = SDL_GetTicks();
	}
	else if (!currentKeyStates[SDL_SCANCODE_D]) {
		rightPressedTime = -1;
	}
	wantsToMove = (upPressedTime >= 0) || (downPressedTime >= 0) || (leftPressedTime >= 0) || (rightPressedTime >= 0);
	if (wantsToMove) {
		desiredDirection = lastPressedDirection();
		desiredSpeed = MAX_SPEED;
	}
	// Set the direction accordingly
	setDirection();
}

void Player::timeSkip(int time) {
	lastMove += time;
	lastStep += time;
}

void Player::render(Renderer* renderer, int cameraX, int cameraY) {
	// Draw the player using the current sprite
	renderer->render(sprites, &spriteSheet[spriteDirection][spriteGait], topLeftX(cameraX), topLeftY(cameraY));
}

int Player::lastPressedDirection() {
	if (upPressedTime > leftPressedTime && upPressedTime > rightPressedTime && upPressedTime > downPressedTime) {
		return UP;
	}
	else if (leftPressedTime > rightPressedTime && leftPressedTime > downPressedTime) {
		return LEFT;
	}
	else if (rightPressedTime > downPressedTime) {
		return RIGHT;
	}
	else {
		return DOWN;
	}
}




