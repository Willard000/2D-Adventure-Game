#include <map>
#include <vector>

#include "Entity.h"
#include "Player.h"

#include "Quadtree.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager {
public:
	typedef std::map<int, Entity *> Entity_Map;

	EntityManager();
	~EntityManager();

	void create(const std::string &type, int type_id, float x = 0.0f, float y = 0.0f);
	void add(Entity *entity);
	void remove(Entity *entity);
	void remove(const std::string &type, int id);
	void clear_entities();
	void clear_spells();

	void update();

	Entity *get_entity(const std::string &type, int id) { return _entities[type][id]; }
	Player *get_player() { return _player; }

	std::map<int, Entity *> *get_entities(const std::string &type) { return &_entities[type]; }
	std::map<std::string, Entity_Map> *get_entities() { return &_entities; }

	friend class ResourceManager;
private:
	void remove_destroyed_entities();
private:
	std::map<std::string, Entity_Map> _entities;
	Player *_player;

	std::vector<Entity *> _entities_to_remove;
};

#endif