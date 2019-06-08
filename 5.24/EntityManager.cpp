#include "EntityManager.h"

EntityManager::EntityManager() {
	_player = new Player(1);
	ComponentLoader::loadComponents(_player);
	//create(TYPE_OBJECT, 1);
}

EntityManager::~EntityManager() {
	delete _player;
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		delete it->second;
	}
	_entities.clear();
}

void EntityManager::create(std::string type, int type_id) {
	Entity *entity = new Entity(type, type_id);
	add(entity);
}

void EntityManager::add(Entity *entity) {
	_entities[entity->get_id()] = entity;
	if (!entity->get_loaded()) {
		ComponentLoader::loadComponents(entity);
	}
}

void EntityManager::remove(Entity *entity) {
	int key = entity->get_id();
	delete _entities[key];
	_entities.erase(key);
}

void EntityManager::update() {
	_player->update();
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		it->second->update();
	}
}