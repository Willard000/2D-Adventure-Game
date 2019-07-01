#include "Entity.h"

#include "ComponentLoader.h"

Entity::Entity(std::string type, int type_id) :
	_type				( type ),
	_type_id			( type_id ),
	_is_loaded			( false )
{
	static int id = 0;
	_id = id++;

	_is_loaded = load_components(this);
}

void Entity::update() {
	for (auto it = _components.begin(); it != _components.end(); it++) {
		if (it->second != nullptr) {
			it->second->update();
		}
	}
}