#ifndef DialogText_H
#define DialogText_H

#include <SDL.h>
#include <SDL_TTF.h>
#include <vector>
#include <string>
#include "renderer.h"

class DialogText {
public:

	std::vector<std::string> prepareChunk(int chunk);

	DialogText(std::vector<std::string> text);
	~DialogText();

	bool advance();

	void update(Renderer * renderer);

	void render(Renderer * renderer);

private:

	const int DRAW_FREQ = 20;			// Number of milliseconds per character drawn
	const int X_START = 10;				// X-index of the pixel where the dialog begins
	const int Y_START = 10;				// Y-ndex of the pixel where the dialog begins
	const int TEXT_WIDTH = 236;			// Width of the text, in pixels
	const int LINE_HEIGHT = 10;			// Height of each line of text, in pixels
	const int FONT_SIZE = 15;			// Size of the font

	std::string fontName;					// Name of the font file
	TTF_Font * font;						// Font object
	std::vector<std::string> dialog;		// The raw dialog, broken into chunks
	std::vector<std::string> currentChunk;	// The current chunk of dialog, split into lines based on the length of the screen
	int chunkIndex;							// The index number of the current chunk of dialog being drawn
	int lineIndex;							// The index number of the current line of dialog being drawn
	int startTime;							// The time when rendering of the current line began
	int numCharacters;						// The number of characters of the current line that are currently being drawn
	bool finishedLine;						// True when we're done rendering the current line
	bool finishedChunk;						// True when we're done rendering the current chunk
	SDL_Texture * previousText;				// The texture that has just been finished being drawn
	SDL_Texture * currentText;				// The texture currently being drawn

};

#endif
