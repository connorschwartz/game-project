#include "visibleobject.h"
#include "util.h"
#include <iostream>

VisibleObject::VisibleObject(int x, int y, int xBlocks, int yBlocks, int width, int height) : SPRITE_WIDTH(width), SPRITE_HEIGHT(height), AREA_XBLOCKS(xBlocks), AREA_YBLOCKS(yBlocks) {
	xPos = xBlockToPixel(x);
	yPos = yBlockToPixel(y);
	xBlock = x;
	yBlock = y;
}

VisibleObject::~VisibleObject() {

}

int VisibleObject::xBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

int VisibleObject::yBlockToPixel(int block) {
	return (block + 1) * Util::BLOCK_SIZE;
}

int VisibleObject::topLeftX(double x, int camX) {
	return x - camX - SPRITE_WIDTH;
}

int VisibleObject::topLeftY(double y, int camY) {
	return y - camY - SPRITE_HEIGHT;
}

int VisibleObject::getCenterX() {
	return (int) xPos - SPRITE_WIDTH / 2;
}

int VisibleObject::getCenterY() {
	return (int) yPos - SPRITE_HEIGHT / 2;
}

int VisibleObject::getPosX() {
	return (int) xPos;
}

int VisibleObject::getPosY() {
	return (int) yPos;
}

int VisibleObject::getWidth() {
	return SPRITE_WIDTH;
}

int VisibleObject::getHeight() {
	return SPRITE_HEIGHT;
}


