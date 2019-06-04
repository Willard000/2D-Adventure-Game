#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

struct Component {
public:
	Component();

	Component(Entity *entity_) {
		entity = entity_;
	}

	virtual void update() = 0;

	Entity *entity;
};

#endif