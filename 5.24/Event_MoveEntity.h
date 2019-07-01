
#include "Event.h"
#include "Entity.h"

#ifndef EVENT_MOVE_ENTITY
#define EVENT_MOVE_ENTITY

struct Event_MoveEntity : public Event {
	Event_MoveEntity(Entity *entity_, int direction_) :
		entity			( entity_ ),
		direction		( direction_ )
	{}

	Entity *entity;
	int direction;
};

#endif