#include <map>
#include <typeindex>
#include <string>

#include "Component.h"

#include "Globals.h"

#ifndef ENITTY_H
#define ENITTY_H

#define GetPosition(entity) static_cast<PositionComponent *>(entity->get_component(typeid(PositionComponent)))
#define GetSprite(entity) static_cast<SpriteComponent *>(entity->get_component(typeid(SpriteComponent)))
#define GetPlayer(entity) static_cast<PlayerComponent *>(entity->get_component(typeid(PlayerComponent)))
#define GetEnemy(entity) static_cast<EnemyComponent *>(entity->get_component(typeid(EnemyComponent)))
#define GetSpell(entity) static_cast<SpellComponent *>(entity->get_component(typeid(SpellComponent)))
#define GetMagic(entity) static_cast<MagicComponent *>(entity->get_component(typeid(MagicComponent)))
#define GetEffect(entity) static_cast<EffectComponent *>(entity->get_component(typeid(EffectComponent)))

class Entity {
public:
	Entity();
	Entity(int type, int type_id);
	Entity(const Entity &rhs);
	Entity &operator=(const Entity &rhs);
	~Entity();

	template <class ComponentType>
	void add_component(ComponentType *component) { _components[typeid(ComponentType)] = component; }

	Component *get_component(std::type_index type) { return _components[type]; }

	virtual void update();

	virtual bool is_collision();

	void clear();

	int get_id() { return _id; }

	int get_type() const { return _type; }
	int get_type_id() { return _type_id; }

	bool get_is_destroyed() { return _is_destroyed; }
	void destroy() { _is_destroyed = true; }

	bool get_is_loaded() { return _is_loaded; }
private:
	int _id; // unique id

	int _type;
	int _type_id;

	bool _is_destroyed;

	bool _is_loaded;

	typedef std::map<std::type_index, Component *> Components;
	Components _components;
};

#endif