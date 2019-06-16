
#include <SDL.h>

#include "Component.h"
#include "Sprite.h"
#include "Timer.h"

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#define MOVE 1
#define CAST 2

struct SpriteComponent : public Component {
	SpriteComponent(Entity *entity, int w, int h, int time_);

	void update();

	void update(Sprite *img);

	Uint8
		m_frame = 0, 
		m_ani = 0, m_ani_prev = 0, 
		m_dir = 0, m_dir_prev = 0;
	SDL_Rect m_pos = { 0, 0, 32, 32 };
	bool m_is_update = false;
	Timer m_time;
};

#endif