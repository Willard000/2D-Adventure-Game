
#include "Event.h"
#include "Entity.h"

#ifndef EVENT_MOVE_ENTITY
#define EVENT_MOVE_ENTITY

struct Event_MoveEntity : public Event {
	Event_MoveEntity(Entity *entity_, int direction_, double time_) {
		entity = entity_;
     	direction = direction_;
		time = time_;
	}

	Entity *entity;
	int direction;
	double time;
};

#endif