#include <string>
#include <array>

#include "Component.h"

#include "Globals.h"

#ifndef ENITTY_H
#define ENITTY_H

#define GetPosition(entity) static_cast<PositionComponent *>(entity->get_component(COMPONENT_POSITION))
#define GetSprite(entity) static_cast<SpriteComponent *>(entity->get_component(COMPONENT_SPRITE))
#define GetPlayer(entity) static_cast<PlayerComponent *>(entity->get_component(COMPONENT_PLAYER))
#define GetEnemy(entity) static_cast<EnemyComponent *>(entity->get_component(COMPONENT_ENEMY))
#define GetSpell(entity) static_cast<SpellComponent *>(entity->get_component(COMPONENT_SPELL))
#define GetMagic(entity) static_cast<MagicComponent *>(entity->get_component(COMPONENT_MAGIC))
#define GetEffect(entity) static_cast<EffectComponent *>(entity->get_component(COMPONENT_EFFECT))
#define GetCombat(entity) static_cast<CombatComponent *>(entity->get_component(COMPONENT_COMBAT))
#define GetItem(entity) static_cast<ItemComponent *>(entity->get_component(COMPONENT_ITEM))
#define GetInteract(entity) static_cast<InteractComponent *>(entity->get_component(COMPONENT_INTERACT))
#define GetNPC(entity) static_cast<NPCComponent *>(entity->get_component(COMPONENT_NPC))

class Entity {
public:
	Entity();
	Entity(int type, int type_id);
	Entity(const Entity &rhs);
	~Entity();

	typedef int Component_Type;

	void add_component(Component *component);

	Component *get_component(const Component_Type &type) { return _components[type]; }

	void update();

	bool is_collision();

	void clear();

	int get_id() { return _id; }

	int get_type() const { return _type; }
	int get_type_id() { return _type_id; }

	bool get_is_destroyed() { return _is_destroyed; }
	void destroy() { _is_destroyed = true; }

	bool get_is_loaded() { return _is_loaded; }

	void set_texture_id(int texture_id) { _texture_id = texture_id; }
	int get_texture_id() { return _texture_id; }
private:
	int _id; // unique id

	int _type;
	int _type_id;

	int _texture_id;

	bool _is_destroyed;

	bool _is_loaded;

	std::array<Component *, TOTAL_COMPONENTS> _components{ nullptr };
};

#endif