#include "EntityManager.h"

#include "Environment.h"
#include "ScriptManager.h"
#include "Window.h"
#include "Log.h"

#include "ComponentLoader.h"
#include "SpriteComponent.h"

EntityManager::EntityManager() :
	_player			( new Player(1) )
{
	Environment::get().get_log()->print("Loading Entity Manager");

	Environment::get().get_window()->get_camera()->center(_player); // move this?
}

EntityManager::~EntityManager() {
	Environment::get().get_log()->print("Closing Entity Manager");

	delete _player;
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			delete itt->second;
		}
	}
	_entities.clear();
}

void EntityManager::create(std::string type, int type_id, double x, double y) {
	Entity *entity = new Entity(type, type_id);

	if (PositionComponent *position = GetPosition(entity)) {
		position->set(x - (position->rect.w / 2), y - (position->rect.h / 2));
	}

	add(entity);

	Environment::get().get_log()->print("Creating Entity - " + type + " " + std::to_string(type_id) + " " + std::to_string(entity->get_id()));
}

void EntityManager::add(Entity *entity) {
	_entities[entity->get_type()][entity->get_id()] = entity;
}

void EntityManager::remove(Entity *entity) {
	if (!entity) {
		return;
	}

	Environment::get().get_log()->print("Deleting Entity - " + entity->get_type() + " " + std::to_string(entity->get_type_id()) + " " + std::to_string(entity->get_id()));

	std::string type = entity->get_type();
	int id = entity->get_id();

	delete _entities[type][id];
	_entities[type].erase(id);
}

void EntityManager::remove(std::string type, int id) {
	Environment::get().get_log()->print("Deleting Entity - " + type + " " + std::to_string(id));

	delete _entities[type][id];
	_entities[type].erase(id);
}

void EntityManager::clear_entities() {
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			delete itt->second;
		}
	}
	_entities.clear();
}

void EntityManager::clear_spells() {
	for (auto it = _entities[TYPE_SPELL].begin(); it != _entities[TYPE_SPELL].end(); it++) {
		delete it->second;
	}
	_entities[TYPE_SPELL].clear();
}

void EntityManager::update() {
	_player->update();
	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			itt->second->update();

			// check to remove entity
			if (itt->second->get_is_destroyed()) {
				_entities_to_remove.push_back(itt->second);
			}
		}
	}

	remove_destroyed_entities();
}

void EntityManager::remove_destroyed_entities() {
	for (auto it = _entities_to_remove.begin(); it != _entities_to_remove.end(); it++) {
		remove(*it);
	}
	_entities_to_remove.clear();
}