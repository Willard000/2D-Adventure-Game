#include <SDL.h>

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "Event_MoveEntity.h"
#include "FileReader.h"

#ifndef MOVEABLE_COMPONENT_H
#define MOVEABLE_COMPONENT_H

struct MoveableComponent : public PositionComponent {
public:
	MoveableComponent(Entity *entity_, double x, double y, int w, int h, double speed_);

	virtual void update() {}

	void move(int dir, double time);
	void move(int dir, double dis, double time);

	double speed;
};

#endif