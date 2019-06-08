
#include "Texture.h"

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite : public Texture {
	Sprite() : Texture() {
		run = 0;
		cast = 0;
		special = 0;
		min_up = 0, max_up = 0;
		min_down = 0, max_down = 0;
		min_left = 0, max_left = 0;
		min_right = 0, max_right = 0;
	}
	~Sprite() {}
	static const unsigned int SPRITE_SHEET_WIDTH = 8;
	unsigned int
		run, cast, special,
		min_up, max_up,
		min_down, max_down,
		min_left, max_left,
		min_right, max_right;
};


#endif
