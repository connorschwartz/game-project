#include <SDL_Mixer.h>
#include <vector>
#include "player.h"
#include "areablocks.h"
#include "renderer.h"
#include "stillobject.h"
#include "npc.h"
#include "dialogtext.h"

class Area {
public:

	// Initialize the components of the area
	void initializeArea(std::string fileName);
	void initializeBackground(std::string fileName);
	void initializeStillObjects(std::string fileName);
	void initializeNPCs(std::string fileName);
	void initializePlayer(std::string fileName);

	Area(std::string fileName, Renderer* renderer);
	~Area();

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
	std::vector<NPC *> npcs;						// NPC objects
	std::vector<SDL_Texture *> npcSprites;			// Sprites for NPCs
	NPC * talkingNPC;								// The NPC who is currently talking
	std::vector<std::string> dialogText;			// Dialog for the talking NPC
	DialogText * currentDialog;						// The current piece of text to render
	int npcOldDirection;							// The NPC's direction, before they are talked to
	int areaWidth;			// Width of the area in blocks
	int areaHeight;			// Height of the area in blocks
	bool dialogMode;		// True if the area is paused for dialog, false otherwise
	int dialogStartTime;	// Timestamp when the dialog-related pause began

	SDL_Texture* grassTextures = nullptr;	// Grass sprite sheet
	SDL_Rect grassTiles[4];					// Area of each grass sprite on the sheet
	int ** grassMap;						// grass sprite number for each block
};


