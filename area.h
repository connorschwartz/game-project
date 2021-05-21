#include <SDL_Mixer.h>
#include <vector>
#include "player.h"
#include "areablocks.h"
#include "renderer.h"
#include "stillobject.h"

class Area {
public:
	Area(std::string fileName, Renderer* renderer);
	~Area();

	// Initialize the components of the area
	void initializeArea(std::string fileName);
	void initializeBackground(std::string fileName);
	void initializeStillObjects(std::string fileName);
	void initializePlayer(std::string fileName);

	// Functions to handle user input
	void handleInput(SDL_Event e);
	void handleKeyStates(const Uint8* currentKeyStates);

	// Move player, NPCs, etc.
	void moveObjects();

	// Draw everything to the screen
	void render(Renderer* renderer);

private:
	Renderer * renderer = nullptr;
	SDL_Texture* testText = nullptr;
	SDL_Texture* testText2 = nullptr;

	Player* player;									// Player object
	AreaBlocks* areaBlocks;							// Mapping of free and used blocks
	std::vector<StillObject *> stillObjects;		// Objects that don't move
	std::vector<SDL_Texture *> stillObjectSprites;	// Sprites for objects that don't move
	int areaWidth;			// Width of the area in blocks
	int areaHeight;			// Height of the area in blocks

	SDL_Texture* grassTextures = nullptr;	// Grass sprite sheet
	SDL_Rect grassTiles[4];					// Area of each grass sprite on the sheet
	int ** grassMap;						// grass sprite number for each block
};


