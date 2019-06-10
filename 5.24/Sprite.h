#include <stdio.h>

#include "Texture.h"

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite : public Texture {

	struct Frame {
		int x;
		int y;
	};

	Sprite() : Texture() {
		run = 0;
		cast = 0;
		special = 0;
		min_up = 0, max_up = 0;
		min_down = 0, max_down = 0;
		min_left = 0, max_left = 0;
		min_right = 0, max_right = 0;
		_frames = nullptr;
	}

	~Sprite() {
		if (_frames != nullptr) {
			delete[] _frames;
		}
	}

	static const unsigned int SPRITE_SHEET_WIDTH = 8;
	unsigned int
		run, cast, special,
		min_up, max_up,
		min_down, max_down,
		min_left, max_left,
		min_right, max_right,
		num_frames;

private:
	Sprite::Frame *_frames; 

public:
	void load_frames() {
		_frames = new Sprite::Frame[num_frames];
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

	Sprite::Frame get_frame(unsigned int index) const {
		if (index < num_frames && index >= 0) {
			return _frames[index];
		}
		printf("Sprite Index at: %d doesn't exist.", index);
		return { 0, 0 };
	}
};


#endif
