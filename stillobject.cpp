#include "stillobject.h"
#include "util.h"

StillObject::StillObject(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int xBlocks, int yBlocks, int blockWidth, int blockHeight, int width, int height) : VisibleObject(x, y, xBlocks, yBlocks, width, height) {
	sprite = s;
}

StillObject::~StillObject() {
	SDL_DestroyTexture(sprite);
}

void StillObject::render(Renderer* renderer, int cameraX, int cameraY) {
	renderer->render(sprite, topLeftX(xPos, cameraX), topLeftY(yPos, cameraY));
}

