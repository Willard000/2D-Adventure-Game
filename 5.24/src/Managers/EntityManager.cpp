#include "EntityManager.h"

#include "Environment.h"
#include "Lua.h"
#include "Window.h"
#include "Log.h"
#include "InputManager.h"
#include "UIManager.h"

#include "ComponentLoader.h"
#include "SpriteComponent.h"
#include "InteractComponent.h"

EntityManager::EntityManager() :
	_player		( new Entity(TYPE_PLAYER, 0) )
{
	Environment::get().get_log()->print("Loading Entity Manager");
}

EntityManager::~EntityManager() {
	Environment::get().get_log()->print("Closing Entity Manager");

	delete _player;
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			delete itt->second;
		}
	}
	_entities.clear();
}

void EntityManager::create_entity(const Type &type, Type_ID type_id, float x, float y) {
	Entity *entity = new Entity(type, type_id);

	if (PositionComponent *position = GetPosition(entity)) {
		position->set(x - (position->rect.w / 2), y - (position->rect.h / 2));
	}

	add_entity(entity);

	create_interact_special_id(entity);

	//Environment::get().get_log()->print("Creating Entity - " + type + " " + std::to_string(type_id) + " " + std::to_string(entity->get_id()));
}

void EntityManager::add_entity(Entity *entity) {
	_entities[entity->get_type()][entity->get_id()] = entity;
}

void EntityManager::remove_entity(Entity *entity) {
	if (!entity) {
		return;
	}

	//Environment::get().get_log()->print("Deleting Entity - " + entity->get_type() + " " + std::to_string(entity->get_type_id()) + " " + std::to_string(entity->get_id()));

	int type = entity->get_type();
	int id = entity->get_id();

	delete _entities[type][id];
	_entities[type].erase(id);
}

void EntityManager::remove_entity(const Type &type, const ID &id) {
	//Environment::get().get_log()->print("Deleting Entity - " + type + " " + std::to_string(id));

	delete _entities[type][id];
	_entities[type].erase(id);
}

void EntityManager::clear_entities() {
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			delete itt->second;
		}
	}
	_entities.clear();
}

void EntityManager::clear_spells() {
	for (auto it = _entities[TYPE_SPELL].begin(); it != _entities[TYPE_SPELL].end(); ++it) {
		delete it->second;
	}
	_entities[TYPE_SPELL].clear();
}

void EntityManager::update_entities() {
	remove_destroyed_entities();

	_player->update();

	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			itt->second->update();

			// check to remove entity
			if (itt->second->get_is_destroyed()) {
				_entities_to_remove.push_back(itt->second);
			}
		}
	}
}

void EntityManager::remove_destroyed_entities() {
	for (auto it = _entities_to_remove.begin(); it != _entities_to_remove.end(); ++it) {
		remove_entity(*it);
	}
	_entities_to_remove.clear();
}

void EntityManager::create_interact_special_id(Entity *entity) {
	if (InteractComponent *interact = GetInteract(entity)) {
		Environment::get().get_ui_manager()->set_current_text("Enter Unique Object Data:");
		Environment::get().get_input_manager()->get_text_input(&interact->data);
	}
}