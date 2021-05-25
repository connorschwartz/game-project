#ifndef Player_H
#define Player_H

#include <SDL.h>
#include "util.h"
#include "areablocks.h"
#include "person.h"
#include "renderer.h"

class Player : public Person {
public:

	Player(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int width, int height);
	~Player();

	// Update the player character based on user input
	void handleKeyStates(const Uint8* currentKeyStates);

	// Draw the player
	void render(Renderer* renderer, int cameraX, int cameraY);

	// Used to stop player movement during dialog
	void timeSkip(int time);

private:

	const int MAX_SPEED = 100;			// Player's speed (pixels/second)
	// width and height of the sprite sheet
	static const int SHEET_WIDTH = 4;
	static const int SHEET_HEIGHT = 3;

	SDL_Rect spriteSheet[SHEET_WIDTH][SHEET_HEIGHT];		// Rectangles to represent the sprites on the sheet
	SDL_Texture* sprites;			// Sprite sheet

};

#endif


