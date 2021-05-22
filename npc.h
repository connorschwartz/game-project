#ifndef NPC_H
#define NPC_H

#include "person.h"
#include "areablocks.h"
#include <string>
#include <sstream>
#include <vector>

class NPC : public Person {

public:

	enum Patterns {
		STAND_STILL,
		RANDOM_WALK,
		WALK_PATH
	};

	NPC(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int width, int height, int speed, std::stringstream& pattern);
	~NPC();

	// Choose movement direction based on the NPC's pattern
	void chooseDirection();

	// Draw the NPC
	void render(Renderer* renderer, int cameraX, int cameraY);

private:

	const int MAX_SPEED;			// NPC's speed (pixels/second)
	// width and height of the sprite sheet
	static const int SHEET_WIDTH = 4;
	static const int SHEET_HEIGHT = 3;

	SDL_Rect spriteSheet[SHEET_WIDTH][SHEET_HEIGHT];		// Rectangles to represent the sprites on the sheet
	SDL_Texture* sprites;			// Sprite sheet

	int movementPattern;			// Pattern of NPC movement decisions (from Patterns enum)
	std::vector<int> movementData;	// Information about NPC movement; format depends on movementPattern
	int lastStepChoiceTime;			// Time when the NPC last tried to choose a step direction

};

#endif
