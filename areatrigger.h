#include <string>
#include "player.h"
#include "renderer.h"

class Area;

class AreaTrigger {
public:

	AreaTrigger(Renderer * renderer, int triggerX, int width, int triggerY, int height, std::string newArea, int startX, int startY);
	~AreaTrigger();

	Area * checkTrigger(Player * player);


private:

	int triggerX, triggerY;			// Coordinates of the lower right part of the trigger within the area
	int width, height;				// Block dimensions of the trigger
	std::string newArea;			// The area that the trigger leads to
	int startX, startY;				// The block where the player starts in the new area

	Renderer * renderer;

};



