#include <iostream>
#include <windows.h>

#include <SDL.h>
#include <SDL_ttf.h>

#include "Engine.h"
#include "Editor.h"

int main(int args, char *argc[]) {

	if (!SDL_Init(SDL_INIT_EVERYTHING)) {
		std::cout << "SDL_LOADED" << std::endl;
	}
	if (!TTF_Init()) {
		std::cout << "SDL_TTF_LOADED" << std::endl;
	}

	int input = 0;
	std::cin >> input;

	if (input == 1) {
		Editor editor;
		editor.run();
	}
	else {
		Engine engine;
		engine.run();
	}

	TTF_Quit();
	SDL_Quit();
	return 0;
}