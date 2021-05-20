#include "areablocks.h"
#include "renderer.h"
#include "visibleobject.h"

class StillObject : public VisibleObject {
public:

	StillObject(SDL_Texture * s, AreaBlocks* areaBlocks, int x, int y, int blockWidth, int blockHeight, int width, int height);
	~StillObject();

	void render(Renderer* renderer, int cameraX, int cameraY);

private:

	SDL_Texture* sprite;

};




