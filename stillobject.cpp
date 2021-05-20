#include "stillobject.h"
#include "util.h"

StillObject::StillObject(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int blockWidth, int blockHeight, int width, int height) : VisibleObject(x, y, width, height) {
	sprite = s;
	blocks->blockRectangle(x, y, blockWidth, blockHeight);
}

StillObject::~StillObject() {
	SDL_DestroyTexture(sprite);
}

void StillObject::render(Renderer* renderer, int cameraX, int cameraY) {
	renderer->render(sprite, topLeftX(xPos, cameraX), topLeftY(yPos, cameraY));
}

