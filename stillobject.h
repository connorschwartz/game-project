#include <vector>
#include "areablocks.h"
#include "renderer.h"
#include "visibleobject.h"
#include "spritetrigger.h"

class StillObject : public VisibleObject {
public:

	StillObject(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int blockWidth, int blockHeight, int width, int height);
	~StillObject();

	void render(Renderer* renderer, int cameraX, int cameraY);

	void update(Player * player);

	void addTrigger(AreaBlocks * blocks, int xBlock, int yBlock, int blockWidth, int blockHeight, int xPixel, int yPixel, int pixelWidth, int pixelHeight);

private:

	SDL_Texture* sprite;
	std::vector<SpriteTrigger *> spriteTriggers;

};




