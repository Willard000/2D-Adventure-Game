#ifndef COMPONENT_H
#define COMPONENT_H

enum
{
	COMPONENT_POSITION,
	COMPONENT_SPRITE,
	COMPONENT_PLAYER,
	COMPONENT_ENEMY,
	COMPONENT_SPELL,
	COMPONENT_MAGIC,
	COMPONENT_EFFECT,
	COMPONENT_COMBAT,
	COMPONENT_ITEM,
	COMPONENT_INTERACT,

	TOTAL_COMPONENTS
};

class Entity;

struct Component {
public:
	Component(Entity *entity_) :
		entity		(entity_)
	{}

	~Component() {}

	// makes new component copy and gives it to new_entity
	virtual Component *copy(Entity *new_entity) const = 0;

	virtual void update() = 0;

	virtual const int get_type() const = 0;

	Entity *entity;
};

#endif