#include "stillobject.h"
#include "util.h"
#include <iostream>

StillObject::StillObject(SDL_Texture * s, AreaBlocks* blocks, int x, int y, int blockWidth, int blockHeight, int lift, int width, int height) : VisibleObject(x, y, width, height) {
	sprite = s;
	spriteLift = lift;
	// Mark the area taken up by the object as used
	blocks->markUsedRectangle(x, y, blockWidth, blockHeight);
}

StillObject::~StillObject() {

}

void StillObject::render(Renderer* renderer, int cameraX, int cameraY) {
	renderer->render(sprite, topLeftX(cameraX), topLeftY(cameraY + spriteLift));
	// Draw the trigger-related objects for any active trigger
	for (auto & trigger : spriteTriggers) {
		if (trigger->isTriggered()) {
			SDL_Rect triggerArea = trigger->getRegion(cameraX, cameraY, xPos, yPos);
			renderer->renderRectangle(triggerArea, 0, 0, 0);
		}
	}
}

void StillObject::update(Player * player) {
	for (auto & trigger : spriteTriggers) {
		trigger->update(player);
	}
}

void StillObject::addTrigger(AreaBlocks * blocks, int x, int y, int blockWidth, int blockHeight, int xPixel, int yPixel, int pixelWidth, int pixelHeight) {
	SpriteTrigger * newTrigger = new SpriteTrigger(xBlock - x, yBlock - y, blockWidth, blockHeight, xPos - xPixel, yPos - yPixel, pixelWidth, pixelHeight);
	blocks->markFreeRectangle(xBlock - x, yBlock - y, blockWidth, blockHeight);
	spriteTriggers.push_back(newTrigger);
}

