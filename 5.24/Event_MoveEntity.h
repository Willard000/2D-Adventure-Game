
#include "Event.h"
#include "Entity.h"

#ifndef EVENT_MOVE_ENTITY
#define EVENT_MOVE_ENTITY

struct Event_MoveEntity : public Event {
	Event_MoveEntity(Entity *entity, int direction) :
		m_entity		( entity ),
		m_direction		( direction )
	{}

	Entity *m_entity;
	int m_direction;
};

#endif