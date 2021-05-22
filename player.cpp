#include <iostream>
#include "util.h"
#include "player.h"


Player::Player(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int width, int height) : Person(blocks, x, y, width, height) {
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
}

Player::~Player() {
	SDL_DestroyTexture(sprites);
}

void Player::handleKeyStates(const Uint8* currentKeyStates) {

	// Determine the direction in which the player wants to move
	wantsToMove = true;
	desiredSpeed = MAX_SPEED;
	if (currentKeyStates[SDL_SCANCODE_W]) {
		desiredDirection = UP;
	}
	else if (currentKeyStates[SDL_SCANCODE_S]) {
		desiredDirection = DOWN;
	}
	else if (currentKeyStates[SDL_SCANCODE_A]) {
		desiredDirection = LEFT;
	}
	else if (currentKeyStates[SDL_SCANCODE_D]) {
		desiredDirection = RIGHT;
	}
	else {
		wantsToMove = false;
		desiredSpeed = 0;
	}
	// Set the direction accordingly
	setDirection();
}

void Player::render(Renderer* renderer, int cameraX, int cameraY) {
	// Draw the player using the current sprite
	renderer->render(sprites, &spriteSheet[spriteDirection][spriteGait], topLeftX(cameraX), topLeftY(cameraY));
}




