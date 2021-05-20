#include <iostream>
#include "util.h"
#include "player.h"

enum GAIT {
	STANDING,
	LEFT_FOOT,
	RIGHT_FOOT
};

enum DIRECTION {
	DOWN,
	RIGHT,
	LEFT,
	UP
};


Player::Player(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int width, int height) : VisibleObject(x, y, width, height) {
	sprites = s;
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
	areaBlocks->block(xBlock, yBlock);
}

Player::~Player() {

}

void Player::handleKeyStates(const Uint8* currentKeyStates) {

	if (!moving) {
		xVel = 0;
		yVel = 0;
		if (currentKeyStates[SDL_SCANCODE_W] && areaBlocks->isFree(xBlock, yBlock - 1)) {
			yVel = -MAX_VELOCITY;
			xSprite = UP;
		}
		else if (currentKeyStates[SDL_SCANCODE_S] && areaBlocks->isFree(xBlock, yBlock + 1)) {
			yVel = MAX_VELOCITY;
			xSprite = DOWN;
		}
		else if (currentKeyStates[SDL_SCANCODE_A] && areaBlocks->isFree(xBlock - 1, yBlock)) {
			xVel = -MAX_VELOCITY;
			xSprite = LEFT;
		}
		else if (currentKeyStates[SDL_SCANCODE_D] && areaBlocks->isFree(xBlock + 1, yBlock)) {
			xVel = MAX_VELOCITY;
			xSprite = RIGHT;
		}
		if (xVel != 0 || yVel != 0) {
			moving = true;
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

	int timePassed = SDL_GetTicks() - lastMove;
	lastMove = SDL_GetTicks();
	if (moving) {
		yPos += (yVel * timePassed) / 1000.0;
		xPos += (xVel * timePassed) / 1000.0;
		if (yVel > 0 && yPos > yBlockToPixel(yBlock + 1)) {
			moving = false;
			yPos = yBlockToPixel(yBlock + 1);
			yBlock++;
		}
		if (yVel < 0 && yPos < yBlockToPixel(yBlock - 1)) {
			moving = false;
			yPos = yBlockToPixel(yBlock - 1);
			yBlock--;
		}
		if (xVel > 0 && xPos > xBlockToPixel(xBlock + 1)) {
			moving = false;
			xPos = xBlockToPixel(xBlock + 1);
			xBlock++;
		}
		if (xVel < 0 && xPos < xBlockToPixel(xBlock - 1)) {
			moving = false;
			xPos = xBlockToPixel(xBlock - 1);
			xBlock--;
		}
	}
	if (lastMove - stepStart > STEP_TIME) {
		ySprite = STANDING;
	}
}

void Player::render(Renderer* renderer, int cameraX, int cameraY) {
	renderer->render(sprites, &spriteSheet[xSprite][ySprite], topLeftX(xPos, cameraX), topLeftY(yPos, cameraY));
}




