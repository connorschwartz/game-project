#include <cmath>
#include <iostream>
#include "util.h"
#include "person.h"


Person::Person(AreaBlocks* blocks, int x, int y, int width, int height) : VisibleObject(x, y, width, height) {
	areaBlocks = blocks;
	xVel = 0;
	yVel = 0;
	moving = false;
	lastMove = SDL_GetTicks();
	lastStep = LEFT_FOOT;
	spriteDirection = DOWN;
	spriteGait = STANDING;
	wantsToMove = false;
	desiredDirection = DOWN;
	desiredSpeed = 0;
}

Person::~Person() {

}

void Person::setDirection() {
	// If the person isn't moving and wants to move, then they can begin the move to another block
	if (!moving && wantsToMove) {
		xVel = 0;
		yVel = 0;
		// If the player tries to move to an open block, mark the block as used and move in that direction
		if (desiredDirection == UP && areaBlocks->isFree(xBlock, yBlock - 1)) {
			areaBlocks->markUsed(xBlock, yBlock - 1);
			yVel = -desiredSpeed;
		}
		else if (desiredDirection == DOWN && areaBlocks->isFree(xBlock, yBlock + 1)) {
			areaBlocks->markUsed(xBlock, yBlock + 1);
			yVel = desiredSpeed;
		}
		else if (desiredDirection == LEFT && areaBlocks->isFree(xBlock - 1, yBlock)) {
			areaBlocks->markUsed(xBlock - 1, yBlock);
			xVel = -desiredSpeed;
		}
		else if (desiredDirection == RIGHT && areaBlocks->isFree(xBlock + 1, yBlock)) {
			areaBlocks->markUsed(xBlock + 1, yBlock);
			xVel = desiredSpeed;
		}
		spriteDirection = desiredDirection;
		// If the person is moving, initialize the step
		if (xVel != 0 || yVel != 0) {
			moving = true;
			wantsToMove = false;
			// Person uses the opposite foot as they did for the previous movement
			if (lastStep == LEFT_FOOT) {
				lastStep = RIGHT_FOOT;
			}
			else {
				lastStep = LEFT_FOOT;
			}
			spriteGait = lastStep;
		}
	}
}

void Person::move() {

	// Time since the move function was last called (to determine how far we should move the person)
	int timePassed = SDL_GetTicks() - lastMove;
	lastMove = SDL_GetTicks();
	if (moving) {
		yPos += (yVel * timePassed) / 1000.0;
		xPos += (xVel * timePassed) / 1000.0;
		// If the person has reached the destination block, complete the step and update the person's position
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
	// Change the person's sprite to the standing sprite halfway through the step
	if ((xVel != 0 && abs(xBlockToPixel(xBlock) - xPos) > Util::BLOCK_SIZE / 2) ||
			(yVel != 0 && abs(yBlockToPixel(yBlock) - yPos) > Util::BLOCK_SIZE / 2)) {
		spriteGait = STANDING;
	}
}

int Person::getIntendedXBlock() {
	int intendedX = xBlock;
	if (moving) {
		if (xVel > 0) intendedX++;
		if (xVel < 0) intendedX--;
	}
	return intendedX;
}

int Person::getIntendedYBlock() {
	int intendedY = yBlock;
	if (moving) {
		if (yVel > 0) intendedY++;
		if (yVel < 0) intendedY--;
	}
	return intendedY;
}

int Person::getSpriteDirection() {
	return spriteDirection;
}

void Person::setSpriteDirection(int direction) {
	spriteDirection = direction;
}
