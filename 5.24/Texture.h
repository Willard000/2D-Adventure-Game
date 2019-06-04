
#include <SDL.h>

#ifndef TEXTURE_H
#define TEXTURE_H

struct Sprite {
	static const unsigned int SPRITE_SHEET_WIDTH = 8;
	unsigned int 
		run = 0, cast = 0, special = 0,
		min_up = 0, max_up = 0,
		min_down = 0, max_down = 0,
		min_left = 0, max_left = 0,
		min_right = 0, max_right = 0;
};

struct Texture {
	Texture() {
		texture = nullptr;
		rect = { 0, 0, 0, 0 };
	}

	~Texture() {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture *texture;
	SDL_Rect rect;
	Sprite sprite;
};

#endif