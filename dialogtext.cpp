#include "dialogtext.h"
#include "util.h"
#include <iostream>


DialogText::DialogText(std::vector<std::string> text) {
	dialog = text;
	currentLine = 0;
	startTime = SDL_GetTicks();
	currentText = nullptr;
}

DialogText::~DialogText() {
	SDL_DestroyTexture(currentText);
}

void DialogText::update(Renderer * renderer) {
	if (currentText == nullptr) {
		currentText = Util::loadTextureFromText(dialog[0], renderer, 15);
	}
	int numCharacters = (SDL_GetTicks() - startTime) / DRAW_SPEED;
}

void DialogText::render(Renderer * renderer, int xPos, int yPos) {
	renderer->render(currentText, xPos, yPos);
}


