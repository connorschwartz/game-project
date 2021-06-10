#include "dialogtext.h"
#include "util.h"
#include <iostream>
#include <sstream>

using namespace std;

vector<string> DialogText::prepareChunk(int chunk) {
	vector<string> lines;
	string line = dialog[chunk];
	stringstream stream(line);
	string nextWord;
	string currentString = "";
	int width;
	// Split the chunk into lines, each of which is small enough to fit on the screen
	while (stream >> nextWord) {
		string nextString = currentString + nextWord + " ";
		TTF_SizeText(font, nextString.c_str(), &width, nullptr);
		if (width > TEXT_WIDTH) {
			lines.push_back(currentString);
			currentString = nextWord + " ";
		}
		else {
			currentString = nextString;
		}
	}
	if (currentString.compare("") != 0) {
		lines.push_back(currentString);
	}
	return lines;
}

DialogText::DialogText(std::vector<std::string> text) {
	fontName = "fonts/AtlantisInternational.ttf";
	font = TTF_OpenFont(fontName.c_str(), FONT_SIZE);
	dialog = text;
	lineIndex = 0;
	chunkIndex = 0;
	startTime = SDL_GetTicks();
	currentChunk = prepareChunk(chunkIndex);
	previousText = nullptr;
	currentText = nullptr;
	numCharacters = 0;
	finishedLine = false;
	finishedChunk = false;
}

DialogText::~DialogText() {
	SDL_DestroyTexture(currentText);
	SDL_DestroyTexture(previousText);
	TTF_CloseFont(font);
	dialog.clear();
	currentChunk.clear();
}

bool DialogText::advance() {
	// Allow the dialog to advance if the current chunk of dialog is completed
	if (finishedChunk) {
		SDL_DestroyTexture(previousText);
		SDL_DestroyTexture(currentText);
		previousText = nullptr;
		currentText = nullptr;
		chunkIndex++;
		// Indicate if the dialog is over and the dialog can end
		if (chunkIndex == dialog.size()) {
			return false;
		}
		lineIndex = 0;
		currentChunk.clear();
		currentChunk = prepareChunk(chunkIndex);
		startTime = SDL_GetTicks();
		finishedChunk = false;
	}
	// Allow the dialog to advance if the current line of dialog is completed
	else if (finishedLine) {
		lineIndex++;
		SDL_DestroyTexture(previousText);
		previousText = currentText;
		currentText = nullptr;
		startTime = SDL_GetTicks();
		finishedLine = false;
	}
	return true;
}

void DialogText::update(Renderer * renderer) {
	// No need to update if the entire line is rendered and we're waiting on user input
	if (finishedLine || finishedChunk) { return; }
	int newCharNum = (SDL_GetTicks() - startTime) / DRAW_FREQ;
	if (newCharNum > currentChunk[lineIndex].length()) newCharNum = currentChunk[lineIndex].length();
	if (numCharacters != newCharNum) {
		SDL_DestroyTexture(currentText);
		numCharacters = newCharNum;
		string partialLine = currentChunk[lineIndex].substr(0, numCharacters);
		currentText = Util::loadTextureFromText(partialLine, renderer, font, {0, 0, 0});
	}
	if (numCharacters == currentChunk[lineIndex].length()) {
		numCharacters = 0;
		if (lineIndex == 0 && currentChunk.size() > 1) {
			lineIndex++;
			previousText = currentText;
			currentText = nullptr;
			startTime = SDL_GetTicks();
		}
		else if (lineIndex == currentChunk.size() - 1){
			lineIndex = 0;
			finishedChunk = true;
		}
		else {
			finishedLine = true;
		}
	}
}

void DialogText::render(Renderer * renderer) {
	renderer->renderRectangle({5, 5, 246, 30}, 255, 255, 255);
	if (previousText == nullptr) {
		if (currentText != nullptr) {
			renderer->render(currentText, X_START, Y_START);
		}
	}
	else {
		renderer->render(previousText, X_START, Y_START);
		if (currentText != nullptr) {
			renderer->render(currentText, X_START, Y_START + LINE_HEIGHT);
		}
	}
}


