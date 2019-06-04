#include <map>

#include "Entity.h"
#include "Player.h"

#include "ComponentLoader.h"
#include "MoveableComponent.h"
#include "SpriteComponent.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	void create(std::string type, int type_id);
	void add(Entity *entity);
	void remove(Entity *entity);

	void update();

	Entity *get(int id) { return _entities[id]; }
	Player *getPlayer() { return _player; }

	friend class ResourceManager;
private:
	std::map<int, Entity *> _entities;
	Player *_player;
};

#endif