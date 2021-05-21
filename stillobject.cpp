#include "stillobject.h"
#include "util.h"

StillObject::StillObject(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int blockWidth, int blockHeight, int width, int height) : VisibleObject(x, y, width, height) {
	sprite = s;
	// Mark the area taken up by the object as used
	blocks->markUsedRectangle(x, y, blockWidth, blockHeight);
}

StillObject::~StillObject() {

}

void StillObject::render(Renderer* renderer, int cameraX, int cameraY) {
	renderer->render(sprite, topLeftX(cameraX), topLeftY(cameraY));
}

