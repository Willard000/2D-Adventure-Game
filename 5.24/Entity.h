#include <unordered_map>
#include <typeindex>
#include <string>

#include "Component.h"
#include "FileReader.h"

#ifndef ENITTY_H
#define ENITTY_H

#define TYPE_PLAYER "Player"
#define TYPE_OBJECT "Object"

#define GetPosition(entity) static_cast<PositionComponent *>(entity->getComponent(typeid(PositionComponent)))
#define GetMoveable(entity) static_cast<MoveableComponent *>(entity->getComponent(typeid(MoveableComponent)))
#define GetSprite(entity) static_cast<SpriteComponent *>(entity->getComponent(typeid(SpriteComponent)))

class Entity {
public:
	Entity(std::string type, int type_id);

	template <class ComponentType>
	void addComponent(ComponentType *component) { _components[typeid(ComponentType)] = component; }

	Component *getComponent(std::type_index type) { return _components[type]; }

	void update();

	int get_id() { return _id; }

	std::string get_type() { return _type; }
	int get_type_id() { return _type_id; }

	void is_loaded() { _isLoaded = true; }
	bool get_loaded() { return _isLoaded; }
private:
	int _id; // unique id

	std::string _type;
	int _type_id;

	bool _isLoaded;

	typedef std::unordered_map<std::type_index, Component *> Components;
	Components _components;
};

#endif