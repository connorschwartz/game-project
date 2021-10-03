#include "menu.h"

Menu::Menu() {

}

Menu::~Menu() {

}

void Menu::handleInput() {

}

void Menu::render(Renderer * renderer) {
	renderer->renderTextBox(20, 20, Util::GAME_WIDTH - 40, Util::GAME_HEIGHT - 40);
}
