#include <SDL.h>

#include "PositionComponent.h"
#include "Entity.h"

#ifndef MOVEABLE_COMPONENT_H
#define MOVEABLE_COMPONENT_H

struct MoveableComponent : public PositionComponent {
public:
	MoveableComponent(Entity *entity_, double x, double y, int w, int h, double speed_);

	virtual void update() {}

	void move(int dir);
	void move(int dir, double dis);

	double speed;
};

#endif