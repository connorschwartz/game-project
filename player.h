#ifndef Player_H
#define Player_H

#include <SDL.h>
#include "util.h"
#include "areablocks.h"
#include "renderer.h"
#include "visibleobject.h"

class Player : public VisibleObject {
public:

	Player(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int xBlocks, int yBlocks, int width, int height);
	~Player();

	void handleKeyStates(const Uint8* currentKeyStates);

	void move();

	void render(Renderer* renderer, int cameraX, int cameraY);

private:

	const int MAX_VELOCITY = 30;
	const int STEP_TIME = 250;
	static const int SHEET_WIDTH = 4;
	static const int SHEET_HEIGHT = 3;

	SDL_Texture* sprites;
	SDL_Rect spriteSheet[SHEET_WIDTH][SHEET_HEIGHT];
	AreaBlocks* areaBlocks;
	double xVel;
	double yVel;
	bool moving;
	int lastMove;
	int stepStart;
	int lastStep;
	int xSprite;
	int ySprite;
};

#endif


