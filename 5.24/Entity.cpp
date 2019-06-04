#include "Entity.h"

Entity::Entity(std::string type, int type_id) {
	static int id = 0;
	_id = id++;
	_type = type;
	_type_id = type_id;
	_isLoaded = false;
}

void Entity::update() {
	for (auto it = _components.begin(); it != _components.end(); it++) {
		if (it->second != nullptr) {
			it->second->update();
		}
	}
}