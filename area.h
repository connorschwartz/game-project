#include <SDL_Mixer.h>
#include <vector>
#include "player.h"
#include "areablocks.h"
#include "renderer.h"
#include "stillobject.h"
#include "npc.h"
#include "dialogtext.h"
#include "background.h"

class AreaTrigger;

class Area {
public:

	// Initialize the components of the area
	void initializeArea(std::string fileName);
	void initializeBackground(std::string fileName);
	void initializeStillObjects(std::string fileName);
	void initializeNPCs(std::string fileName);
	void initializePlayer(std::string fileName, int playerX, int playerY, std::string playerSpriteDirection);
	void initializeAreaTriggers(std::string fileName);

	Area(std::string fileName, Renderer* renderer, int playerX, int playerY, std::string playerSpriteDirection);
	~Area();

	// Allows all moving objects to be paused and resumed
	void pauseMovement();
	void resumeMovement();

	// Handles fading out when the area is left
	bool isFaded();
	void fadeOut();

	// Pause everything to run
	void initiateDialog();

	// Check if there is something with dialog in front of the player, and initialize dialog if so
	void checkDialog();

	// End the dialog
	void concludeDialog();

	// Functions to handle user input
	void handleInput(SDL_Event e);
	void handleKeyStates(const Uint8* currentKeyStates);

	// Move player, NPCs, etc.
	void moveObjects();

	// Check if the player is moving to a different area
	Area * checkAreaTriggers();

	// Draw everything to the screen
	void render(Renderer* renderer, SDL_Window * window);

private:
	Renderer * renderer = nullptr;

	Player* player;									// Player object
	AreaBlocks* areaBlocks;							// Mapping of free and used blocks
	Background* background;							// Handles the rendering of the background textures
	std::vector<StillObject *> stillObjects;		// Objects that don't move
	std::vector<SDL_Texture *> stillObjectSprites;	// Sprites for objects that don't move
	std::vector<NPC *> npcs;						// NPC objects
	std::vector<SDL_Texture *> npcSprites;			// Sprites for NPCs
	std::vector<AreaTrigger *> areaTriggers;		// Triggers to handle moving between areas
	NPC * talkingNPC;								// The NPC who is currently talking
	std::vector<std::string> dialogText;			// Dialog for the talking NPC
	DialogText * currentDialog;						// The current piece of text to render
	int npcOldDirection;							// The NPC's direction, before they are talked to
	int areaWidth;			// Width of the area in blocks
	int areaHeight;			// Height of the area in blocks
	bool dialogMode;		// True if the area is in dialog mode, false otherwise
	bool paused;			// True if the objects are paused, false otherwise
	int pauseStartTime;		// Timestamp when the pause began
	int alpha;				// 0 when the screen is fully faded to black, 255 for full color
	int fadeOutStartTime;	// Time when fadein began
	int fadeInStartTime;	// Time when fadeout began
};

