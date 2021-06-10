#include "areatrigger.h"
#include "area.h"

AreaTrigger::AreaTrigger(Renderer * renderer, int triggerX, int width, int triggerY, int height, std::string newArea, int startX, int startY, std::string startDirection) : renderer{renderer}, triggerX{triggerX}, width{width}, triggerY{triggerY}, height{height}, newArea(newArea), startX{startX}, startY{startY}, startSpriteDirection(startDirection) {

}

AreaTrigger::~AreaTrigger() {

}

Area * AreaTrigger::checkTrigger(Player * player) {
	int playerX = player->getBlockX();
	int playerY = player->getBlockY();
	if (playerX <= triggerX && playerX > triggerX - width && playerY <= triggerY && playerY > triggerY - height) {
		Area * area = new Area(newArea, renderer, startX, startY, startSpriteDirection);
		return area;
	}
	return nullptr;
}


