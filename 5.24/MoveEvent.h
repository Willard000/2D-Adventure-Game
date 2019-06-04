
#include "Event.h"
#include "Entity.h"

#ifndef MOVE_EVENT_H
#define MOVE_EVENT_H

class Entity;

struct MoveEvent : public Event {
	MoveEvent(Entity *entity_, int direction_, double time_) {
		entity = entity_;
		direction = direction_;
		time = time_;
	}

	Entity *entity;
	int direction;
	double time;

	enum {
		MOVE_UP = 1,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		MOVE_UPLEFT,
		MOVE_UPRIGHT,
		MOVE_DOWNLEFT,
		MOVE_DOWNRIGHT
	};
};

#endif