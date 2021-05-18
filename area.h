#include <SDL_Mixer.h>
#include "player.h"
#include "areablocks.h"
#include "renderer.h"

class Area {
public:
	Area(std::string fileName, Renderer* renderer);
	~Area();

	void handleInput(SDL_Event e);
	void handleKeyStates(const Uint8* currentKeyStates);

	void moveObjects();

	void render(Renderer* renderer);

private:
	SDL_Texture* grassTextures = nullptr;
	SDL_Texture* testText = nullptr;
	SDL_Texture* testText2 = nullptr;

	Mix_Music* testMusic = nullptr;
	Mix_Chunk* testSound = nullptr;

	Player* player;
	AreaBlocks* areaBlocks;
	int xBlocks;
	int yBlocks;
	int xPixels;
	int yPixels;

	SDL_Rect grassTiles[4];
	int ** grassMap;
};


