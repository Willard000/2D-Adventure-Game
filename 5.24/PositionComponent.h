#include <SDL.h>

#include "Component.h"
#include "FileReader.h"

#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

struct PositionComponent : public Component {
public:
	PositionComponent(Entity *entity_, double x_, double y_, int w_, int h_);

	virtual void update() {}

	void to_SDL_Rect();

	double pos_x;
	double pos_y;
	SDL_Rect position;
};

#endif