#ifndef Player_H
#define Player_H

#include <SDL.h>
#include "util.h"
#include "areablocks.h"
#include "renderer.h"

class Player {
public:

	Player(Renderer* renderer, AreaBlocks* areaBlocks, int x, int y, int xBlocks, int yBlocks);
	~Player();

	void handleKeyStates(const Uint8* currentKeyStates);

	void move();

	void render(Renderer* renderer, int cameraX, int cameraY);

	int xBlockToPixel(int block);
	int yBlockToPixel(int block);
	int topLeftX(double x, int camX);
	int topLeftY(double y, int camY);
	int getCenterX();
	int getCenterY();
	int getPosX();
	int getPosY();
	int getWidth();
	int getHeight();

private:

	const int MAX_VELOCITY = 30;
	const int STEP_TIME = 250;
	const int SPRITE_HEIGHT = 24;
	const int SPRITE_WIDTH = 16;
	static const int SHEET_WIDTH = 4;
	static const int SHEET_HEIGHT = 3;

	SDL_Texture* sprites;
	SDL_Rect spriteSheet[SHEET_WIDTH][SHEET_HEIGHT];
	AreaBlocks* areaBlocks;
	double xPos;
	double yPos;
	double xVel;
	double yVel;
	int xBlock;
	int yBlock;
	int areaXBlocks;
	int areaYBlocks;
	bool moving;
	int lastMove;
	int stepStart;
	int lastStep;
	int xSprite;
	int ySprite;
};

#endif


