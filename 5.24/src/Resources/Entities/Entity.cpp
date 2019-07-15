#include "Entity.h"

#include "ComponentLoader.h"

#include "PositionComponent.h"

int create_id() {
	static int id = 0;
	return id++;
}

Entity::Entity() :
	_id				( create_id() ),
	_type			( " " ),
	_type_id		( -1 ),
	_is_destroyed	( false ),
	_is_loaded		( false )
{}

Entity::Entity(std::string type, int type_id) :
	_id					( create_id() ),
	_type				( type ),
	_type_id			( type_id ),
	_is_destroyed		( false ),
	_is_loaded			( false )
{
	_is_loaded = load_components(this);
}

Entity::Entity(const Entity &rhs) :
	_id				( create_id() ),
	_type			( rhs._type ),
	_type_id		( rhs._type_id ),
	_is_destroyed	( rhs._is_destroyed ),
	_is_loaded		( false )
{
	_is_loaded = load_components(this);
}

Entity &Entity::operator=(const Entity &rhs) {
	_id = create_id();
	_type = rhs._type;
	_type_id = rhs._type_id;
	_is_destroyed = rhs._is_destroyed;

	_is_loaded = load_components(this);

	return *this;
}

Entity::~Entity() {
	for (auto it = _components.begin(); it != _components.end(); ++it) {
		delete it->second;
	}
}

void Entity::update() {
	for (auto it = _components.begin(); it != _components.end(); ++it) {
		if (it->second != nullptr) {
			it->second->update();
		}
	}
}