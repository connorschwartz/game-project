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

	SDL_Texture* sprite;
	SDL_Rect spriteSheet[3];
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

	const int MAX_VELOCITY = 30;
	const int SPRITE_HEIGHT = 24;
	const int SPRITE_WIDTH = 16;
};

#endif


