#include <string>
#include <sstream>
#include <iostream>
#include <SDL.h>
#include <fstream>
#include <algorithm>
#include "util.h"
#include "area.h"
#include "player.h"
#include "npc.h"
#include "dialogtext.h"
#include "areatrigger.h"

using namespace std;

void Area::initializeArea(string fileName) {
	ifstream data;
	string line;
	data.open(fileName, ios_base::out);
	getline(data, line);
	stringstream stream(line);
	stream >> areaWidth;
	stream >> areaHeight;
	areaBlocks = new AreaBlocks(areaWidth, areaHeight);
	// Read in the default blocked areas, if provided. Otherwise no areas will be blocked by default.
	getline(data, line);
	if (line.compare("Default:") == 0) {
		getline(data, line);
		int value;
		for (int i = 0; i < areaHeight; i++) {
			stringstream stream(line);
			bool block = true;
			int currentX = 0;
			while (stream >> value) {
				currentX = currentX + value;
				if (block) {
					areaBlocks->markUsedRectangle(currentX - 1, i, value, 1);
					block = false;
				}
				else {
					block = true;
				}
			}
			getline(data, line);
		}
	}
	data.close();
}

void Area::initializeBackground(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	int sheetWidth, sheetHeight;
	data >> sheetWidth >> sheetHeight;
	background = new Background(data, renderer, areaWidth, areaHeight, sheetWidth, sheetHeight);
	// Optional list of background tile types which are blocked by default
	string line;
	getline(data, line);
	if (getline(data, line) && !line.empty() && line.compare("Blocked:") == 0) {
		getline(data, line);
		// Read from the file which tile types are automatically blocked
		int tileTypes [sheetWidth * sheetHeight] = { };
		stringstream stream(line);
		int tileNum;
		while (stream >> tileNum) {
			tileTypes[tileNum] = 1;
		}
		// Mark the appropriate area blocks as used
		for (int i = 0; i < areaHeight; i++) {
			for (int j = 0; j < areaWidth; j++) {
				if (tileTypes[background->getTileType(i, j)] == 1) {
					areaBlocks->markUsed(i, j);
				}
			}
		}
	}
	data.close();
}

void Area::initializeStillObjects(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	string line;
	getline(data, line);	// Skip the first line, since it's just the header for the Images section
	StillObject * newObject;
	// Load in the sprite for each filename listed in the data file, stopping when we reach the Objects section
	getline(data, line);
	while (line.compare("Objects:") != 0) {
		stringstream stream(line);
		int index;
		string filename;
		stream >> index >> filename;
		// Get the sprite and add it to the list
		SDL_Texture * sprite = Util::loadTexture(filename, renderer);
		stillObjectSprites.push_back(sprite);
		getline(data, line);
	}
	// Load in the data for each object or rectangular group of objects
	int spriteNum;		// Index number of the sprite for the object
	int startX;			// x-coordinate of the bottom right block of the first object
	int width;			// Block width of each object
	int xSpacing;		// Distance between objects in the x direction
	int endX;			// x-coordinate of the bottom right block of the final object
	int startY;			// y-coordinate of the bottom right block of the first object
	int height;			// Block width of each object
	int ySpacing;		// Distance between objects in the y direction
	int endY;			// y-coordinate of the bottom right block of the final object
	int spriteLift;		// Distance from the bottom of the object where the sprite begins
	int pixelWidth;		// Width of the sprite, in pixels
	int pixelHeight;	// Height of the sprite, in pixels
	while (getline(data, line) && !line.empty()) {
		stringstream stream(line);
		stream >> spriteNum >> startX >> width >> xSpacing >> endX >> startY >> height >> ySpacing >> endY >> spriteLift >> pixelWidth >> pixelHeight;
		// Create the object, or the group of objects if applicable
		for (int i = startX; i <= endX; i += width + xSpacing) {
			for (int j = startY; j <= endY; j += height + ySpacing) {
				newObject = new StillObject(stillObjectSprites[spriteNum], areaBlocks, i, j, width, height, spriteLift, pixelWidth, pixelHeight);
				// If there's more data, attach an event trigger to the object
				while (!stream.eof()) {
					string separatorChar;
					stream >> separatorChar;
					int xBlock, yBlock, blockWidth, blockHeight, xPixel, yPixel, width, height;
					stream >> xBlock >> yBlock >> blockWidth >> blockHeight >> xPixel >> yPixel >> width >> height;
					newObject->addTrigger(areaBlocks, xBlock, yBlock, blockWidth, blockHeight, xPixel, yPixel, width, height);
				}
				stillObjects.push_back(newObject);
			}
		}
	}
	data.close();
}

void Area::initializeNPCs(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	string line;
	getline(data, line);	// Skip the first line, since it's just the header for the Images section
	string terminator = "NPCs:";
	NPC * newNPC;
	// Load in the sprite for each filename listed in the data file, stopping when we reach the NPCs section
	getline(data, line);
	while (line.compare(terminator) != 0) {
		stringstream stream(line);
		int index;
		string filename;
		stream >> index >> filename;
		// Get the sprite and add it to the list
		SDL_Texture * sprite = Util::loadTexture(filename, renderer);
		npcSprites.push_back(sprite);
		getline(data, line);
	}
	// Load in the data for each object or rectangular group of objects
	int spriteNum;			// Index number of the sprite for the object
	int xBlock;				// x-coordinate of the NPC's initial block
	int yBlock;				// y-coordinate of the NPC's initial block
	string spriteDirection;	// Direction the NPC is initially facing
	int pixelWidth;			// Width of the sprite, in pixels
	int pixelHeight;		// Height of the sprite, in pixels
	int speed;				// NPC's speed, in pixels/second
	int numLines;			// Number of dialog lines
	while (getline(data, line) && !line.empty()) {
		stringstream stream(line);
		stream >> spriteNum >> xBlock >> yBlock >> spriteDirection >> pixelWidth >> pixelHeight >> speed >> numLines;
		std::vector<std::string> npcDialog;
		std::string nextLine;
		for (int i = 0; i < numLines; i++) {
			getline(data, nextLine);
			npcDialog.push_back(nextLine);
		}
		// Create the NPC, passing the rest of the line, since it contains the NPC's behavior data
		newNPC = new NPC(npcSprites[spriteNum], areaBlocks, xBlock, yBlock, spriteDirection, pixelWidth, pixelHeight, speed, stream, npcDialog);
		npcs.push_back(newNPC);
	}
	data.close();
}

void Area::initializePlayer(string fileName, int playerX, int playerY, std::string playerSpriteDirection) {
	ifstream data;
	data.open(fileName, ios_base::out);
	// Get the player's sprite
	string playerFilename;
	data >> playerFilename;
	SDL_Texture * playerSprite = Util::loadTexture(playerFilename, renderer);
	// Get the player's sprite width and height
	int pixelWidth, pixelHeight;
	data >> pixelWidth;
	data >> pixelHeight;
	player = new Player(playerSprite, areaBlocks, playerX, playerY, playerSpriteDirection, pixelWidth, pixelHeight);
	data.close();
}

void Area::initializeAreaTriggers(string fileName) {
	ifstream data;
	data.open(fileName);
	string line;
	int triggerX, triggerY;		// The coordinates of the lower right corner of the trigger
	int width, height;			// The dimensions of the trigger
	string newArea;				// The name of the area led to by the trigger
	int startX, startY;			// The starting coordinates for the player in the new area
	string startDirection;		// The starting direction for the player in the new area
	while (getline(data, line) && !line.empty()) {
		stringstream stream(line);
		stream >> triggerX >> width >> triggerY >> height >> newArea >> startX >> startY >> startDirection;
		areaTriggers.push_back(new AreaTrigger(renderer, triggerX, width, triggerY, height, newArea, startX, startY, startDirection));
	}
}

Area::Area(string directory, Renderer * r, int playerX, int playerY, string playerSpriteDirection) {
	renderer = r;
	dialogMode = false;
	pauseMenuMode = false;
	paused = false;
	currentDialog = nullptr;
	pauseMenu = nullptr;
	fadeInStartTime = SDL_GetTicks();
	alpha = 0;
	fadeOutStartTime = -1;
	initializeArea(directory + "/area.txt");
	initializeBackground(directory + "/background.txt");
	initializeStillObjects(directory + "/still_objects.txt");
	initializeNPCs(directory + "/npcs.txt");
	initializePlayer(directory + "/player.txt", playerX, playerY, playerSpriteDirection);
	initializeAreaTriggers(directory + "/area_triggers.txt");
}

Area::~Area() {
	for (int i = 0; i < stillObjectSprites.size(); i++) {
		SDL_DestroyTexture(stillObjectSprites[i]);
	}
	for (int i = 0; i < npcSprites.size(); i++) {
		SDL_DestroyTexture(npcSprites[i]);
	}
	delete background;
    delete player;
	stillObjects.clear();
	npcs.clear();
	areaTriggers.clear();
}

void Area::pauseMovement() {
	pauseStartTime = SDL_GetTicks();
	paused = true;
}

void Area::resumeMovement() {
	paused = false;
	// Change the timer counts on the objects and allow them to move again
	int timeUpdate = SDL_GetTicks() - pauseStartTime;
	fadeInStartTime += timeUpdate;
	player->timeSkip(timeUpdate);
	for (auto & npc : npcs) {
		npc->timeSkip(timeUpdate);
	}
}

bool Area::isFaded() {
	if (fadeOutStartTime > 0 && alpha == 0) return true;
	return false;
}

void Area::fadeOut() {
	fadeOutStartTime = SDL_GetTicks();
}

void Area::initiateDialog() {
	pauseMovement();
	dialogMode = true;
	currentDialog = new DialogText(talkingNPC->getDialog());
	int playerDirection = player->getSpriteDirection();
	npcOldDirection = talkingNPC->getSpriteDirection();
	int npcDirection;
	if (playerDirection == Person::LEFT) {
		npcDirection = Person::RIGHT;
	}
	else if (playerDirection == Person::RIGHT) {
		npcDirection = Person::LEFT;
	}
	else if (playerDirection == Person::UP) {
		npcDirection = Person::DOWN;
	}
	else if (playerDirection == Person::DOWN) {
		npcDirection = Person::UP;
	}
	talkingNPC->setSpriteDirection(npcDirection);
}

void Area::checkDialog() {
	if (!paused) {
		int xBlock = player->getBlockX();
		int yBlock = player->getBlockY();
		int playerDirection = player->getSpriteDirection();
		// Get the space that the player is facing
		if (playerDirection == Person::LEFT) {
			xBlock--;
		}
		else if (playerDirection == Person::RIGHT) {
			xBlock++;
		}
		else if (playerDirection == Person::UP) {
			yBlock--;
		}
		else if (playerDirection == Person::DOWN) {
			yBlock++;
		}
		// See if there's an NPC in that space
		talkingNPC = nullptr;
		for (auto & npc : npcs) {
			if (npc->getBlockX() == xBlock && npc->getBlockY() == yBlock) {
				talkingNPC = npc;
				break;
			}
		}
		if (talkingNPC != nullptr) {
			initiateDialog();
		}
	}
}

void Area::concludeDialog() {
	dialogMode = false;
	delete currentDialog;
	// Return the talking NPC to their original orientation
	talkingNPC->setSpriteDirection(npcOldDirection);
	// Unpause all of the objects that had to stop moving for the dialog
	resumeMovement();
}

void Area::openMenu() {
	if (!dialogMode) {
		pauseMenuMode = true;
		pauseMenu = new Menu();
		pauseMovement();
	}
}

void Area::closeMenu() {
	pauseMenuMode = false;
	delete pauseMenu;
	resumeMovement();
}

void Area::handleInput(SDL_Event e) {

	// When the user hits the P key...
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_p) {
		// Toggle the menu open/closed
		if (pauseMenuMode) {
			closeMenu();
		}
		else {
			openMenu();
		}
	}
	// When the user hits the space bar...
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE) {
		// Advance the dialog if we're in dialog mode
		if (dialogMode) {
			// If we're in the final step of the dialog, end it
			if (!currentDialog->advance()) {
				concludeDialog();
			}
		}
		// Try to initiate dialog mode otherwise
		else {
			checkDialog();
		}
	}
	// If we're in menu mode, pass the key to the menu
	if (pauseMenuMode) {
		pauseMenu->handleInput();
	}
}

void Area::handleKeyStates(const Uint8* currentKeyStates) {
	if (!paused) {
		player->handleKeyStates(currentKeyStates);
	}
}

void Area::moveObjects() {
	if (dialogMode) {
		currentDialog->update(renderer);
	}
	else if (!paused) {
		player->move();
		for (auto & npc : npcs) {
			npc->chooseDirection();
			npc->move();
		}
		for (auto & object : stillObjects) {
			object->update(player);
		}
	}
	// Update fade levels when fading in and out
	if (fadeInStartTime >= 0) {
		alpha = (SDL_GetTicks() - fadeInStartTime) / 3;
		if (alpha > 255) {
			alpha = 255;
			fadeInStartTime = -1;
		}
	}
	if (fadeOutStartTime >= 0) {
		alpha = 255 - (SDL_GetTicks() - fadeOutStartTime) / 3;
		if (alpha < 0) {
			alpha = 0;
			fadeInStartTime = -1;
		}
	}
}

Area * Area::checkAreaTriggers() {
	Area * newArea = nullptr;
	for (auto & trigger: areaTriggers) {
		newArea = trigger->checkTrigger(player);
		if (newArea != nullptr) break;
	}
	return newArea;
}

void Area::render(Renderer* renderer, SDL_Window * window) {
	renderer->setAlpha(alpha);
	// Place the camera so that the player is in the center
	int playerX = player->getCenterX();
	int playerY = player->getCenterY();
	int cameraX = playerX - Util::GAME_WIDTH / 2;
	int cameraY = playerY - Util::GAME_HEIGHT / 2;
	// Adjust the camera so it only shows areas within the bounds
	if (cameraX < 0) cameraX = 0;
	if (cameraY < 0) cameraY = 0;
	if (cameraX + Util::GAME_WIDTH > areaWidth * Util::BLOCK_SIZE) cameraX = areaWidth * Util::BLOCK_SIZE - Util::GAME_WIDTH;
	if (cameraY + Util::GAME_HEIGHT > areaHeight * Util::BLOCK_SIZE) cameraY = areaHeight * Util::BLOCK_SIZE - Util::GAME_HEIGHT;
	// If the area is too small for scrolling, set negative camera values
	if (Util::GAME_WIDTH >= areaWidth * Util::BLOCK_SIZE) cameraX = (areaWidth * Util::BLOCK_SIZE - Util::GAME_WIDTH) / 2;
	if (Util::GAME_HEIGHT >= areaHeight * Util::BLOCK_SIZE) cameraY = (areaHeight * Util::BLOCK_SIZE - Util::GAME_HEIGHT) / 2;
	// Render the background
	background->render(renderer, cameraX, cameraY);
	// Draw the player and objects
	vector<VisibleObject *> objectsToDraw;
	objectsToDraw.push_back(player);
	player->render(renderer, cameraX, cameraY);
	for (auto & npc : npcs) {
		// Only bother drawing onscreen NPCs
		if (npc->isOnscreen(cameraX, cameraY)) {
			objectsToDraw.push_back(npc);
		}
	}
	for (auto & item : stillObjects) {
		// Only bother drawing onscreen objects
		if (item->isOnscreen(cameraX, cameraY)) {
			objectsToDraw.push_back(item);
		}
	}
	// Sort the onscreen objects by vertical position, so that they will overlap properly
	sort(objectsToDraw.begin(), objectsToDraw.end(), VisibleObject::compare);
	for (auto & item : objectsToDraw) {
		item->render(renderer, cameraX, cameraY);
	}
	if (dialogMode) {
		currentDialog->render(renderer);
	}
	if (pauseMenuMode) {
		pauseMenu->render(renderer);
	}
}
