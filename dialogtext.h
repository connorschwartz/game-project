#ifndef DialogText_H
#define DialogText_H

#include <SDL.h>
#include <vector>
#include <string>
#include "renderer.h"

class DialogText {
public:

	DialogText(std::vector<std::string> text);
	~DialogText();

	void update(Renderer * renderer);

	void render(Renderer * renderer, int xPos, int yPos);

private:

	const int DRAW_SPEED = 5;			// Number of milliseconds per character drawn

	std::vector<std::string> dialog;	// The raw dialog
	int currentLine;					// The current line of dialog being drawn
	int startTime;						// The time when rendering of the current line began
	SDL_Texture * currentText;			// The texture currently being drawn

};

#endif
