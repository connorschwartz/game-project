#include <iostream>
#include "util.h"
#include "player.h"


Player::Player(Renderer* renderer, AreaBlocks* blocks, int x, int y, int xBlocks, int yBlocks) {
	sprite = Util::loadTexture("images/player.png", renderer);
	spriteSheet[0] = {0,0,16,24};
	spriteSheet[1] = {0,24,16,24};
	spriteSheet[2] = {0,48,16,24};
	areaBlocks = blocks;
	xPos = xBlockToPixel(x);
	yPos = yBlockToPixel(y);
	xVel = 0;
	yVel = 0;
	xBlock = x;
	yBlock = y;
	areaXBlocks = xBlocks;
	areaYBlocks = yBlocks;
	moving = false;
	lastMove = SDL_GetTicks();
}

Player::~Player() {
	SDL_DestroyTexture(sprite);
}

void Player::handleKeyStates(const Uint8* currentKeyStates) {

	if (!moving) {
		xVel = 0;
		yVel = 0;
		if (currentKeyStates[SDL_SCANCODE_W] && areaBlocks->isFree(xBlock, yBlock - 1)) {
			yVel = -MAX_VELOCITY;
		}
		else if (currentKeyStates[SDL_SCANCODE_S] && areaBlocks->isFree(xBlock, yBlock + 1)) {
			yVel = MAX_VELOCITY;
		}
		else if (currentKeyStates[SDL_SCANCODE_A] && areaBlocks->isFree(xBlock - 1, yBlock)) {
			xVel = -MAX_VELOCITY;
		}
		else if (currentKeyStates[SDL_SCANCODE_D] && areaBlocks->isFree(xBlock + 1, yBlock)) {
			xVel = MAX_VELOCITY;
		}
		if (xVel != 0 || yVel != 0) {
			moving = true;
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
}

void Player::render(Renderer* renderer, int cameraX, int cameraY) {
	int spriteNum = 0;
	if (moving) {
		spriteNum = 1;
	}
	renderer->render(sprite, &spriteSheet[spriteNum], topLeftX(xPos, cameraX), topLeftY(yPos, cameraY));
}

int Player::xBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

int Player::yBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

int Player::topLeftX(double x, int camX) {
	return x - camX - SPRITE_WIDTH;
}

int Player::topLeftY(double y, int camY) {
	return y - camY - SPRITE_HEIGHT;
}

int Player::getCenterX() {
	return (int) xPos - SPRITE_WIDTH / 2;
}

int Player::getCenterY() {
	return (int) yPos - SPRITE_HEIGHT / 2;
}

int Player::getPosX() {
	return (int) xPos;
}

int Player::getPosY() {
	return (int) yPos;
}

int Player::getWidth() {
	return SPRITE_WIDTH;
}

int Player::getHeight() {
	return SPRITE_HEIGHT;
}




