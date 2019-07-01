#include <unordered_map>
#include <typeindex>
#include <string>

#include "Component.h"

#ifndef ENITTY_H
#define ENITTY_H

#define TYPE_PLAYER "Player"
#define TYPE_OBJECT "Object"

#define GetPosition(entity) static_cast<PositionComponent *>(entity->get_component(typeid(PositionComponent)))
#define GetSprite(entity) static_cast<SpriteComponent *>(entity->get_component(typeid(SpriteComponent)))

class Entity {
public:
	Entity(std::string type, int type_id);

	template <class ComponentType>
	void add_component(ComponentType *component) { _components[typeid(ComponentType)] = component; }

	Component *get_component(std::type_index type) { return _components[type]; }

	void update();

	int get_id() { return _id; }

	std::string get_type() { return _type; }
	int get_type_id() { return _type_id; }

	bool get_is_loaded() { return _is_loaded; }
private:
	int _id; // unique id

	std::string _type;
	int _type_id;

	bool _is_loaded;

	typedef std::unordered_map<std::type_index, Component *> Components;
	Components _components;
};

#endif