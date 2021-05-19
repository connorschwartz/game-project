#ifndef VisibleObject_H
#define VisibleObject_H

#include "renderer.h"

class VisibleObject {
public:

	VisibleObject(int x, int y, int xBlocks, int yBlocks, int width, int height);
	virtual ~VisibleObject() = 0;

	virtual void render(Renderer* renderer, int cameraX, int cameraY) = 0;

	int xBlockToPixel(int block);
	int yBlockToPixel(int block);
	int topLeftX(double x, int camX);
	int topLeftY(double y, int camY);
	int getCenterX();
	int getCenterY();
	int getPosX();
	int getPosY();
	int getWidth();
	int getHeight();

protected:

	const int SPRITE_WIDTH;
	const int SPRITE_HEIGHT;
	const int AREA_XBLOCKS;
	const int AREA_YBLOCKS;

	double xPos;
	double yPos;
	int xBlock;
	int yBlock;
};

#endif


