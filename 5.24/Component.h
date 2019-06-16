#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

struct Component {
public:
	Component();

	Component(Entity *entity) {
		m_entity = entity;
	}

	virtual void update() = 0;

	Entity *m_entity;
};

#endif