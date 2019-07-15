
#include <SDL.h>

#ifndef TEXTURE_H
#define TEXTURE_H

struct Texture {
	~Texture() {
		if (texture != nullptr) {
			SDL_DestroyTexture(texture);
		}
	}

	SDL_Texture *texture = nullptr;
	SDL_Rect rect = { 0, 0, 0, 0 };
};
#endif