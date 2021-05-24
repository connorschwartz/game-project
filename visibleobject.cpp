#include "visibleobject.h"
#include "util.h"

VisibleObject::VisibleObject(int x, int y, int width, int height) : SPRITE_WIDTH(width), SPRITE_HEIGHT(height) {
	xPos = xBlockToPixel(x);
	yPos = yBlockToPixel(y);
	xBlock = x;
	yBlock = y;
}

VisibleObject::~VisibleObject() {

}

// Returns the pixel x-coordinate corresponding to the right size of the given block
int VisibleObject::xBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

// Returns the pixel y-coordinate corresponding to the right size of the given block
int VisibleObject::yBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

// Returns the leftmost pixel of the object, given the camera location
int VisibleObject::topLeftX(int camX) {
	return xPos - camX - SPRITE_WIDTH;
}

// Returns the topmost pixel of the object, given the camera location
int VisibleObject::topLeftY(int camY) {
	return yPos - camY - SPRITE_HEIGHT;
}

// Returns the x-coordinate of the object's center
int VisibleObject::getCenterX() {
	return (int) xPos - SPRITE_WIDTH / 2;
}

// Returns the y-coordinate of the object's center
int VisibleObject::getCenterY() {
	return (int) yPos - SPRITE_HEIGHT / 2;
}

// Determines if the object is onscreen
bool VisibleObject::isOnscreen(int cameraX, int cameraY) {
	if ((xPos < cameraX) || (yPos < cameraY) || (xPos > cameraX + Util::GAME_WIDTH + SPRITE_WIDTH) ||
			(yPos > cameraY + Util::GAME_HEIGHT + SPRITE_HEIGHT)) {
		return false;
	}
	return true;
}

// Compare by y position, so that sprites can be drawn from bottom to top and overlap properly
bool VisibleObject::compare(VisibleObject * object1, VisibleObject * object2) {
	return (object1->getPosY() < object2->getPosY());
}

// Accessors

int VisibleObject::getPosX() {
	return (int) xPos;
}

int VisibleObject::getPosY() {
	return (int) yPos;
}

int VisibleObject::getBlockX() {
	return xBlock;
}

int VisibleObject::getBlockY() {
	return yBlock;
}

int VisibleObject::getWidth() {
	return SPRITE_WIDTH;
}

int VisibleObject::getHeight() {
	return SPRITE_HEIGHT;
}


