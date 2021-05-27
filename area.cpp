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

using namespace std;

void Area::initializeArea(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	string text1;
	string text2;
	data >> text1;
	data >> text2;
	data >> areaWidth;
	data >> areaHeight;
	areaBlocks = new AreaBlocks(areaWidth, areaHeight);
	data.close();
}

void Area::initializeBackground(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	int sheetWidth, sheetHeight;
	data >> sheetWidth >> sheetHeight;
	background = new Background(data, renderer, areaWidth, areaHeight, sheetWidth, sheetHeight);
	data.close();
}

void Area::initializeStillObjects(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	string line;
	getline(data, line);	// Skip the first line, since it's just the header for the Images section
	string terminator = "Objects:";
	StillObject * newObject;
	// Load in the sprite for each filename listed in the data file, stopping when we reach the Objects section
	getline(data, line);
	while (line.compare(terminator) != 0) {
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
	int endX;			// x-coordinate of the bottom right block of the final object
	int startY;			// y-coordinate of the bottom right block of the first object
	int height;			// Block width of each object
	int endY;			// y-coordinate of the bottom right block of the final object
	int pixelWidth;		// Width of the sprite, in pixels
	int pixelHeight;	// Height of the sprite, in pixels
	while (getline(data, line) && !line.empty()) {
		stringstream stream(line);
		stream >> spriteNum >> startX >> width >> endX >> startY >> height >> endY >> pixelWidth >> pixelHeight;
		// Create the object, or the group of objects if applicable
		for (int i = startX; i <= endX; i += width) {
			for (int j = startY; j <= endY; j += height) {
				newObject = new StillObject(stillObjectSprites[spriteNum], areaBlocks, i, j, width, height, pixelWidth, pixelHeight);
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
	int spriteNum;		// Index number of the sprite for the object
	int xBlock;			// x-coordinate of the NPC's initial block
	int yBlock;			// y-coordinate of the NPC's initial block
	int pixelWidth;		// Width of the sprite, in pixels
	int pixelHeight;	// Height of the sprite, in pixels
	int speed;			// NPC's speed, in pixels/second
	int numLines;		// Number of dialog lines
	while (getline(data, line) && !line.empty()) {
		stringstream stream(line);
		stream >> spriteNum >> xBlock >> yBlock >> pixelWidth >> pixelHeight >> speed >> numLines;
		std::vector<std::string> npcDialog;
		std::string nextLine;
		for (int i = 0; i < numLines; i++) {
			getline(data, nextLine);
			npcDialog.push_back(nextLine);
		}
		// Create the NPC, passing the rest of the line, since it contains the NPC's behavior data
		newNPC = new NPC(npcSprites[spriteNum], areaBlocks, xBlock, yBlock, pixelWidth, pixelHeight, speed, stream, npcDialog);
		npcs.push_back(newNPC);
	}
	data.close();
}

void Area::initializePlayer(string fileName) {
	ifstream data;
	data.open(fileName, ios_base::out);
	// Get the player's sprite
	string playerFilename;
	data >> playerFilename;
	SDL_Texture * playerSprite = Util::loadTexture(playerFilename, renderer);
	// Get the player's initial position
	int xPos, yPos;
	data >> xPos;
	data >> yPos;
	// Get the player's sprite width and height
	int pixelWidth, pixelHeight;
	data >> pixelWidth;
	data >> pixelHeight;
	player = new Player(playerSprite, areaBlocks, xPos, yPos, pixelWidth, pixelHeight);
	data.close();
}

Area::Area(string directory, Renderer * r) {
	renderer = r;
	dialogMode = false;
	currentDialog = nullptr;
	initializeArea(directory + "/area.txt");
	initializeBackground(directory + "/background.txt");
	initializeStillObjects(directory + "/still_objects.txt");
	initializeNPCs(directory + "/npcs.txt");
	initializePlayer(directory + "/player.txt");
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
}

void Area::initiateDialog() {
	dialogMode = true;
	dialogStartTime = SDL_GetTicks();
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
	if (!dialogMode) {
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
	// Objects should have stayed still during the dialog - update all of them accordingly
	int timeUpdate = SDL_GetTicks() - dialogStartTime;
	player->timeSkip(timeUpdate);
	for (auto & npc : npcs) {
		npc->timeSkip(timeUpdate);
	}
}

void Area::handleInput(SDL_Event e) {
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
}

void Area::handleKeyStates(const Uint8* currentKeyStates) {
	if (!dialogMode) {
		player->handleKeyStates(currentKeyStates);
	}
}

void Area::moveObjects() {
	if (dialogMode) {
		currentDialog->update(renderer);
	}
	else {
		player->move();
		for (auto & npc : npcs) {
			npc->chooseDirection();
			npc->move();
		}
	}
}

void Area::render(Renderer* renderer) {
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
	SDL_Rect bgClip = {cameraX, cameraY, Util::GAME_WIDTH, Util::GAME_HEIGHT};
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
}
