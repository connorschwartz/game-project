#include <iostream>
#include "util.h"
#include "player.h"

// Stages of the player's gait when walking
enum GAIT {
	STANDING,
	LEFT_FOOT,
	RIGHT_FOOT
};

// Direction the player is facing
enum DIRECTION {
	DOWN,
	RIGHT,
	LEFT,
	UP
};


Player::Player(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int width, int height) : VisibleObject(x, y, width, height) {
	sprites = s;
	// Create rectangles to represent each sprite on the player's sprite sheet
	for (int i = 0; i < SHEET_WIDTH; i++) {
		for (int j = 0; j < SHEET_HEIGHT; j++) {
			spriteSheet[i][j] = {i * SPRITE_WIDTH, j * SPRITE_HEIGHT, SPRITE_WIDTH, SPRITE_HEIGHT};
		}
	}
	areaBlocks = blocks;
	xVel = 0;
	yVel = 0;
	moving = false;
	lastMove = SDL_GetTicks();
	stepStart = SDL_GetTicks();
	lastStep = LEFT_FOOT;
	xSprite = DOWN;
	ySprite = STANDING;
	// Mark the player's starting position as used
	areaBlocks->markUsed(xBlock, yBlock);
}

Player::~Player() {
	SDL_DestroyTexture(sprites);
}

void Player::handleKeyStates(const Uint8* currentKeyStates) {

	// If the player isn't moving, then they can begin the move to another block
	if (!moving) {
		xVel = 0;
		yVel = 0;
		// If the player tries to move to an open block, mark the block as used and move in that direction
		if (currentKeyStates[SDL_SCANCODE_W]) {
			xSprite = UP;
			if (areaBlocks->isFree(xBlock, yBlock - 1)) {
				areaBlocks->markUsed(xBlock, yBlock - 1);
				yVel = -MAX_VELOCITY;
			}
		}
		else if (currentKeyStates[SDL_SCANCODE_S]) {
			xSprite = DOWN;
			if (areaBlocks->isFree(xBlock, yBlock + 1)) {
				areaBlocks->markUsed(xBlock, yBlock + 1);
				yVel = MAX_VELOCITY;
			}
		}
		else if (currentKeyStates[SDL_SCANCODE_A]) {
			xSprite = LEFT;
			if (areaBlocks->isFree(xBlock - 1, yBlock)) {
				areaBlocks->markUsed(xBlock - 1, yBlock);
				xVel = -MAX_VELOCITY;
			}
		}
		else if (currentKeyStates[SDL_SCANCODE_D]) {
			xSprite = RIGHT;
			if (areaBlocks->isFree(xBlock + 1, yBlock)) {
				areaBlocks->markUsed(xBlock + 1, yBlock);
				xVel = MAX_VELOCITY;
			}
		}
		// If the player is moving, initialize the step
		if (xVel != 0 || yVel != 0) {
			moving = true;
			// Player uses the opposite foot as they did for the previous movement
			if (lastStep == LEFT_FOOT) {
				lastStep = RIGHT_FOOT;
			}
			else {
				lastStep = LEFT_FOOT;
			}
			stepStart = SDL_GetTicks();
			ySprite = lastStep;
		}
	}
}

void Player::move() {

	// Time since the move function was last called (to determine how far we should move the player)
	int timePassed = SDL_GetTicks() - lastMove;
	lastMove = SDL_GetTicks();
	if (moving) {
		yPos += (yVel * timePassed) / 1000.0;
		xPos += (xVel * timePassed) / 1000.0;
		// If the player has reached the destination block, complete the step and update the player's position
		if (yVel > 0 && yPos > yBlockToPixel(yBlock + 1)) {
			moving = false;
			areaBlocks->markFree(xBlock, yBlock);
			yPos = yBlockToPixel(yBlock + 1);
			yBlock++;
		}
		if (yVel < 0 && yPos < yBlockToPixel(yBlock - 1)) {
			moving = false;
			areaBlocks->markFree(xBlock, yBlock);
			yPos = yBlockToPixel(yBlock - 1);
			yBlock--;
		}
		if (xVel > 0 && xPos > xBlockToPixel(xBlock + 1)) {
			moving = false;
			areaBlocks->markFree(xBlock, yBlock);
			xPos = xBlockToPixel(xBlock + 1);
			xBlock++;
		}
		if (xVel < 0 && xPos < xBlockToPixel(xBlock - 1)) {
			moving = false;
			areaBlocks->markFree(xBlock, yBlock);
			xPos = xBlockToPixel(xBlock - 1);
			xBlock--;
		}
	}
	// Change the player's sprite to the standing sprite part-way through the step
	if (lastMove - stepStart > STEP_TIME) {
		ySprite = STANDING;
	}
}

void Player::render(Renderer* renderer, int cameraX, int cameraY) {
	// Draw the player using the current sprite
	renderer->render(sprites, &spriteSheet[xSprite][ySprite], topLeftX(cameraX), topLeftY(cameraY));
}




