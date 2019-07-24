
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
	SpriteComponent(Entity *entity_, int w_, int h_, int time_);

	virtual void update();

	void update(Sprite *img);

	Uint8
		frame,
		ani, ani_prev, 
		dir, dir_prev;

	SDL_Rect pos;
	bool is_update;
	Timer time;
};

#endif