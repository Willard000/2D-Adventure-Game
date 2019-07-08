#include <map>

#include "Entity.h"
#include "Player.h"

#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

class EntityManager {
public:
	typedef std::map<int, Entity *> Entity_Map;

	EntityManager();
	~EntityManager();

	void create(std::string type, int type_id, double x = 0, double y = 0);
	void add(Entity *entity);
	void remove(Entity *entity);
	void remove(std::string type, int id);
	void clear_entities();
	void clear_spells();

	void update();

	Entity *get_entity(std::string type, int id) { return _entities[type][id]; }
	Player *get_player() { return _player; }

	std::map<int, Entity *> *get_entities(std::string type) { return &_entities[type]; }
	std::map<std::string, Entity_Map> *get_entities() { return &_entities; }

	friend class ResourceManager;
private:
	std::map<std::string, Entity_Map> _entities;
	Player *_player;
};

#endif