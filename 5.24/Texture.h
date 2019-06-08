
#include <SDL.h>

#ifndef TEXTURE_H
#define TEXTURE_H

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
};
#endif