#include <map>

#include "Entity.h"
#include "Player.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager {
public:
	EntityManager();
	~EntityManager();

	void create(std::string type, int type_id, double x = 0, double y = 0);
	void add(Entity *entity);
	void remove(Entity *entity);
	void remove(std::string type, int id);
	void clear_entities();

	void update();

	Entity *get_entity(int id) { return _entities[id]; }
	Player *get_player() { return _player; }

	std::map<int, Entity *> *get_entities() { return &_entities; }

	friend class ResourceManager;
private:
	std::map<int, Entity *> _entities;
	Player *_player;
};

#endif