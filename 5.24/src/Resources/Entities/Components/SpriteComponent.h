
#include <SDL.h>

#include "Component.h"
#include "Sprite.h"
#include "Timer.h"

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#define MOVE 1
#define CAST 2
#define IDLE 3

struct SpriteComponent : public Component {
	SpriteComponent(Entity *entity_, int w_, int h_, int time_, int cast_time_);
	SpriteComponent(Entity *new_entity, const SpriteComponent &rhs);
	SpriteComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_SPRITE; }

	void update(Sprite *img);

	Uint8
		frame,
		ani, ani_prev, 
		dir, dir_prev;

	SDL_Rect pos;
	bool is_update;
	Timer time;
	Timer cast_timer;
};

#endif