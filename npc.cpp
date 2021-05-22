#include "npc.h"
#include <iostream>

NPC::NPC(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int width, int height, int speed, std::stringstream& pattern) : Person(blocks, x, y, width, height), MAX_SPEED(speed) {
	sprites = s;
	// Create rectangles to represent each sprite on the player's sprite sheet
	for (int i = 0; i < SHEET_WIDTH; i++) {
		for (int j = 0; j < SHEET_HEIGHT; j++) {
			spriteSheet[i][j] = {i * SPRITE_WIDTH, j * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
		}
	}
	areaBlocks = blocks;
	// Mark the NPC's starting position as used
	areaBlocks->markUsed(xBlock, yBlock);
	std::string movementType;
	pattern >> movementType;
	// For random walks, get the data for the rectangle in which the NPC will move
	if (movementType.compare("RANDOM_WALK") == 0) {
		movementPattern = RANDOM_WALK;
		int topLeftX, topLeftY, bottomLeftX, bottomLeftY, timeBetweenSteps;
		pattern >> topLeftX >> topLeftY >> bottomLeftX >> bottomLeftY >> timeBetweenSteps;
		movementData.push_back(topLeftX);
		movementData.push_back(topLeftY);
		movementData.push_back(bottomLeftX);
		movementData.push_back(bottomLeftY);
		movementData.push_back(timeBetweenSteps);
	}
	else if (movementType.compare("WALK_PATH") == 0) {
		movementPattern = WALK_PATH;
	}
	else {
		movementPattern = STAND_STILL;
	}
	lastStepChoiceTime = SDL_GetTicks();
}

NPC::~NPC() {

}

void NPC::chooseDirection() {
	if (movementPattern == RANDOM_WALK) {
		// If the NPC hasn't moved in a while, choose one of the four directions at random
		if (lastStepChoiceTime + movementData[4] < SDL_GetTicks()) {
			lastStepChoiceTime = SDL_GetTicks();
			wantsToMove = true;
			desiredSpeed = MAX_SPEED;
			desiredDirection = rand() % 4;
			// Make sure the desired direction wouldn't put the NPC out of range
			if ((desiredDirection == UP && yBlock == movementData[1]) ||
					(desiredDirection == DOWN && yBlock == movementData[3]) ||
					(desiredDirection == LEFT && xBlock == movementData[0]) ||
					(desiredDirection == RIGHT && xBlock == movementData[2])) {
				wantsToMove = false;
			}
		}
	}
	setDirection();
}

void NPC::render(Renderer* renderer, int cameraX, int cameraY) {
	// Draw the player using the current sprite
	renderer->render(sprites, &spriteSheet[spriteDirection][spriteGait], topLeftX(cameraX), topLeftY(cameraY));
}


