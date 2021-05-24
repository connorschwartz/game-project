#ifndef VisibleObject_H
#define VisibleObject_H

#include "renderer.h"

class VisibleObject {
public:

	VisibleObject(int x, int y, int width, int height);
	virtual ~VisibleObject() = 0;

	virtual void render(Renderer* renderer, int cameraX, int cameraY) = 0;

	// Conversion between blocks and pixels (bottom right corner of block)
	int xBlockToPixel(int block);
	int yBlockToPixel(int block);

	// Returns sprite's top left pixel coordinates
	int topLeftX(int camX);
	int topLeftY(int camY);

	// Returns sprite's center pixel coordinates
	int getCenterX();
	int getCenterY();

	// Determines if the object is onscreen
	bool isOnscreen(int cameraX, int cameraY);

	static bool compare(VisibleObject * object1, VisibleObject * object2);

	// Accessors
	int getPosX();
	int getPosY();
	int getBlockX();
	int getBlockY();
	int getWidth();
	int getHeight();

protected:

	// Size of the sprite
	const int SPRITE_WIDTH;
	const int SPRITE_HEIGHT;

	// Pixel location of the lower right corner of the sprite
	double xPos;
	double yPos;

	// Lower right block of the sprite
	int xBlock;
	int yBlock;
};

#endif


