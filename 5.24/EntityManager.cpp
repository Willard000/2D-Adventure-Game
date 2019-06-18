#include "EntityManager.h"

#include "Environment.h"
#include "ScriptManager.h"
#include "WindowManager.h"
#include "LogManager.h"

#include "ComponentLoader.h"
#include "SpriteComponent.h"

EntityManager::EntityManager() :
	_player			( new Player(1) )
{
	Environment::get().getLogManager()->log("Loading Entity Manager");

	loadComponents(_player);
	Environment::get().getWindowManager()->getWindow()->getCamera()->center(_player); // move this?
	GetPosition(_player)->pos_x = 200;
	GetPosition(_player)->rect.x = 200;
	GetPosition(_player)->pos_y = 200;
	GetPosition(_player)->rect.y = 200;

	create(TYPE_OBJECT, 1);
}

EntityManager::~EntityManager() {
	Environment::get().getLogManager()->log("Closing Entity Manager");

	delete _player;
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		delete it->second;
	}
	_entities.clear();
}

void EntityManager::create(std::string type, int type_id) {
	Entity *entity = new Entity(type, type_id);
	add(entity);

	Environment::get().getLogManager()->log("Creating Entity - " + type + " " + std::to_string(type_id) + " " + std::to_string(entity->get_id()));
}

void EntityManager::add(Entity *entity) {
	_entities[entity->get_id()] = entity;
	if (!entity->get_loaded()) {
		loadComponents(entity);
	}
}

void EntityManager::remove(Entity *entity) {
	Environment::get().getLogManager()->log("Deleting Entity - " + entity->get_type() + " " + std::to_string(entity->get_type_id()) + " " + std::to_string(entity->get_id()));

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