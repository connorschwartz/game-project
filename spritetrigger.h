#include <SDL.h>
#include "player.h"

class SpriteTrigger {
public:

	SpriteTrigger(int xBlock, int yBlock, int blockWidth, int blockHeight, int xPixel, int yPixel, int pixelWidth, int pixelHeight);
	~SpriteTrigger();

	// Check if the sprite has been triggered based on the player's information
	void update(Player * player);

	// Checks if the player has hit the trigger
	bool isTriggered();

	// Returns the area that should be blacked out if the trigger is hit
	SDL_Rect getRegion(int cameraX, int cameraY, int objectX, int objectY);


private:

	// Location and dimensions of the trigger within the sprite
	int xBlock;
	int yBlock;
	int blockWidth;
	int blockHeight;
	int xPixel;
	int yPixel;
	int pixelWidth;
	int pixelHeight;

	// True if the trigger is active, false otherwise
	bool triggered;

};


