#include <map>
#include <vector>

#include "Entity.h"

#include "Quadtree.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

namespace emanager {
	typedef int Type;
	typedef int Type_ID;
	typedef int ID;
}

using namespace emanager;

class EntityManager {
public:
	typedef std::map<ID, Entity *> Entity_Map;

	EntityManager();
	~EntityManager();

	void create(const Type &type, Type_ID type_id, float x = 0.0f, float y = 0.0f);
	void add(Entity *entity);
	void remove(Entity *entity);
	void remove(const Type &type, const ID &id);
	void clear_entities();
	void clear_spells();

	void update();

	Entity *get_entity(const Type &type, const ID &id) { return _entities[type][id]; }
	Entity *get_player() { return _player; }

	std::map<ID, Entity *> *get_entities(const Type &type) { return &_entities[type]; }
	std::map<Type, Entity_Map> *get_entities() { return &_entities; }

	friend class ResourceManager;
private:
	void remove_destroyed_entities();
private:
	std::map<Type, Entity_Map> _entities;
	Entity *_player;

	std::vector<Entity *> _entities_to_remove;
};

#endif