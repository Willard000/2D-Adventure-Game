
#include <SDL.h>

#include "Component.h"
#include "Sprite.h"
#include "Timer.h"

#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#define MOVE 1
#define CAST 2

struct SpriteComponent : public Component {
	SpriteComponent(Entity *entity_, int w_, int h_, int time_);

	void update();

	void update(Sprite *img);

	Uint8
		frame = 0, 
		ani = 0, ani_prev = 0, 
		dir = 0, dir_prev = 0;

	SDL_Rect pos = { 0, 0, 32, 32 };
	bool is_update = false;
	Timer time;
};

#endif