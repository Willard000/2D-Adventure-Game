#include "Sprite.h"

#include <stdio.h>

void Sprite::load_frames() {
	_frames = new Frame[num_frames];
	int x = 0, y = 0;

	for (unsigned int i = 0; i < num_frames; i++) {
		_frames[i].x = x;
		_frames[i].y = y;

		x += rect.w;
		if (i != 0 && (i % SPRITE_SHEET_WIDTH) == 0) {
			x = 0;
			y += rect.h;
		}
	}
}

const Sprite::Frame Sprite::get_frame(Uint8 index) {
	if (index < num_frames && index >= 0) {
		return _frames[index];
	}
	return { 0, 0 };
}
