#include "npc.h"
#include <iostream>

NPC::NPC(SDL_Texture * s, AreaBlocks* blocks, int x, int y, std::string spriteDirection, int width, int height, int speed, std::stringstream& pattern, std::vector<std::string> npcDialog) : Person(blocks, x, y, spriteDirection, width, height), MAX_SPEED(speed) {
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
	dialog = npcDialog;
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
	// For walking paths, we have an arbitrarily long series of Direction Distance pairs
	else if (movementType.compare("WALK_PATH") == 0) {
		movementPattern = WALK_PATH;
		std::string dir;
		int distance;
		while (pattern >> dir >> distance) {
			int direction;
			if (dir.compare("LEFT") == 0) {
				direction = LEFT;
			}
			else if (dir.compare("DOWN") == 0) {
				direction = DOWN;
			}
			else if (dir.compare("RIGHT") == 0) {
				direction = RIGHT;
			}
			else {
				direction = UP;
			}
			movementData.push_back(direction);
			movementData.push_back(distance);
		}
		// Second to last value in the movement data is the current step in the movement. Initialize to the final step,
		// so we can start from the beginning
		int lastStep = (movementData.size() / 2) - 1;
		movementData.push_back(lastStep);
		// Last value in the movement data is the block (either x or y) that marks the end of the current step
		// Initialize it to the current value - essentially we are acting as though the NPC has just finished the pattern
		desiredDirection = movementData[lastStep * 2];
		if (desiredDirection == LEFT || desiredDirection == RIGHT) {
			movementData.push_back(xBlock);
		}
		else {
			movementData.push_back(yBlock);
		}
	}
	else {
		movementPattern = STAND_STILL;
	}
	lastStepChoiceTime = SDL_GetTicks();
}

NPC::~NPC() {
	dialog.clear();
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
	else if (movementPattern == WALK_PATH) {
		// NPCs that walk a path always want to move
		desiredSpeed = MAX_SPEED;
		wantsToMove = true;
		// Move to the next step if we've reached the end of the current step
		int currentStep = movementData[movementData.size() - 2];
		int endBlock = movementData[movementData.size() - 1];
		if (((desiredDirection == LEFT || desiredDirection == RIGHT) && endBlock == xBlock) ||
				((desiredDirection == UP || desiredDirection == DOWN) && endBlock == yBlock)) {
			movementData[movementData.size() - 2]++;
			// Loop around when the end of the pattern is reached
			if (movementData[movementData.size() - 2] == (movementData.size() / 2) - 1) {
				movementData[movementData.size() - 2] = 0;
			}
			// Update all data for the new step
			currentStep = movementData[movementData.size() - 2];
			desiredDirection = movementData[currentStep * 2];
			int stepSize = movementData[currentStep * 2 + 1];
			if (desiredDirection == LEFT) {
				movementData[movementData.size() - 1] = xBlock - stepSize;
			}
			else if (desiredDirection == UP) {
				movementData[movementData.size() - 1] = yBlock - stepSize;
			}
			else if (desiredDirection == RIGHT) {
				movementData[movementData.size() - 1] = xBlock + stepSize;
			}
			else {
				movementData[movementData.size() - 1] = yBlock + stepSize;
			}
		}
	}
	setDirection();
}

void NPC::timeSkip(int time) {
	lastMove += time;
	lastStep += time;
	lastStepChoiceTime += time;
}

void NPC::render(Renderer* renderer, int cameraX, int cameraY) {
	// Draw the player using the current sprite
	renderer->render(sprites, &spriteSheet[spriteDirection][spriteGait], topLeftX(cameraX), topLeftY(cameraY));
}

std::vector<std::string> NPC::getDialog() {
	return dialog;
}


