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

	void create_entity(const Type &type, Type_ID type_id, float x = 0.0f, float y = 0.0f);
	void add_entity(Entity *entity);
	void remove_entity(Entity *entity);
	void remove_entity(const Type &type, const ID &id);
	void clear_entities();
	void clear_spells();

	void update_entities();

	Entity *get_entity(const Type &type, const ID &id) { return _entities[type][id]; }
	Entity *get_player() { return _player; }

	std::map<ID, Entity *> *get_entities(const Type &type) { return &_entities[type]; }
	std::map<Type, Entity_Map> *get_entities() { return &_entities; }

	void create_interact_special_id(Entity *entity);

	friend class ResourceManager;
private:
	void remove_destroyed_entities();
private:
	std::map<Type, Entity_Map> _entities;
	Entity *_player;

	std::vector<Entity *> _entities_to_remove;
};

#endif