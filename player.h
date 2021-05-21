#ifndef Player_H
#define Player_H

#include <SDL.h>
#include "util.h"
#include "areablocks.h"
#include "renderer.h"
#include "visibleobject.h"

class Player : public VisibleObject {
public:

	Player(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int width, int height);
	~Player();

	// Update the player character based on user input
	void handleKeyStates(const Uint8* currentKeyStates);

	// Move the player
	void move();

	// Draw the player
	void render(Renderer* renderer, int cameraX, int cameraY);

private:

	const int MAX_VELOCITY = 30;			// Player's speed (pixels/second)
	const int STEP_TIME = 250;				// time in milliseconds before the player switches back to the standing sprite when stepping
	// width and height of the sprite sheet
	static const int SHEET_WIDTH = 4;
	static const int SHEET_HEIGHT = 3;

	SDL_Texture* sprites;			// Sprite sheet
	SDL_Rect spriteSheet[SHEET_WIDTH][SHEET_HEIGHT];		// Rectangles to represent the sprites on the sheet
	AreaBlocks* areaBlocks;			// Used to track which blocks the player can move to
	double xVel;		// x direction velocity
	double yVel;		// y direction velocity
	bool moving;		// True if the player is in the process of stepping, false otherwise
	int lastMove;		// Time when the player was last moved on the screen
	int stepStart;		// Time when the player began their current step
	int lastStep;		// Indicates whether the player used their left or right foot during the last step
	int xSprite;		// x-coordinate of the player's current sprite on the sheet
	int ySprite;		// y-coordinate of the player's current sprite on the sheet
};

#endif


