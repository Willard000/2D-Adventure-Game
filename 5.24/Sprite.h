#include <stdio.h>

#include "Texture.h"

#ifndef SPRITE_H
#define SPRITE_H

struct Sprite : public Texture {

	struct Frame {
		int x;
		int y;
	};

	Sprite() :
		Texture(),
		m_run		( 0 ),
		m_cast		( 0 ),
		m_special	( 0 ),
		m_min_up	( 0 ),	 m_max_up	 ( 0 ),
		m_min_down	( 0 ),	 m_max_down	 ( 0 ),
		m_min_left	( 0 ),	 m_max_left	 ( 0 ),
		m_min_right	( 0 ),	 m_max_right ( 0 ),
		_frames		( nullptr )
	{}

	~Sprite() {
		if (_frames != nullptr) {
			delete[] _frames;
		}
	}

	static const unsigned int SPRITE_SHEET_WIDTH = 8;
	unsigned int
		m_run, m_cast, m_special,
		m_min_up, m_max_up,
		m_min_down, m_max_down,
		m_min_left, m_max_left,
		m_min_right, m_max_right,
		m_num_frames;

private:
	Frame *_frames; 
public:
	void load_frames() {
		_frames = new Frame[m_num_frames];
		int x = 0, y = 0;

		for (unsigned int i = 0; i < m_num_frames; i++) {
			_frames[i].x = x;
			_frames[i].y = y;

			x += rect.w;
			if (i != 0 && (i % SPRITE_SHEET_WIDTH) == 0) {
				x = 0;
				y += rect.h;
			}
		}
	}

	Frame get_frame(unsigned int index) const {
		if (index < m_num_frames && index >= 0) {
			return _frames[index];
		}
		printf("Sprite Index at: %d doesn't exist.", index);
		return { 0, 0 };
	}
};


#endif
