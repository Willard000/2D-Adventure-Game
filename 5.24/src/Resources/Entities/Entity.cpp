#include "Entity.h"

#include "ComponentLoader.h"

#include "PositionComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "SpellComponent.h"
#include "NPCComponent.h"

int create_id() {
	static int id = 0;
	return id++;
}

Entity::Entity() :
	_id				( create_id() ),
	_type			( 0 ),
	_type_id		( 0 ),
	_texture_id     ( 0 ),
	_is_destroyed	( false ),
	_is_loaded		( false )
{}

Entity::Entity(int type, int type_id) :
	_id					( create_id() ),
	_type				( type ),
	_type_id			( type_id ),
	_texture_id         ( 0 ),
	_is_destroyed		( false ),
	_is_loaded			( false )
{
	_is_loaded = load_components(this);
}

Entity::Entity(const Entity &rhs) :
	_id				( create_id() ),
	_type			( rhs._type ),
	_type_id		( rhs._type_id ),
	_texture_id     ( rhs._texture_id ),
	_name			( rhs._name ),
	_is_destroyed	( rhs._is_destroyed ),
	_is_loaded		( rhs._is_loaded )
{
	for (unsigned int i = 0; i < rhs._components.size(); ++i) {
		if (rhs._components[i]) {
			_components[i] = rhs._components[i]->copy(this);
		}
	}
}

Entity::~Entity() {
	clear();
}

void Entity::update() {
	for (auto &component : _components) {
		if(component)
			component->update();
	}
}

bool Entity::is_collision() {
	switch (_type) {
	case TYPE_PLAYER:
		return GetPlayer(this)->is_collision();
	case TYPE_ENEMY:
		return GetEnemy(this)->is_collision();
	case TYPE_SPELL:
		return GetSpell(this)->is_collision();
	case TYPE_NPC:
		return GetNPC(this)->is_collision();
	}

	return false;
}

void Entity::clear() {
	for (auto it = _components.begin(); it != _components.end(); ++it) {
		if (*it) {
			delete *it;
			*it = nullptr;
		}
	}
}

void Entity::add_component(Component *component) { 
	if (!_components[component->get_type()]) {
		_components[component->get_type()] = component;
	}
}