#include "Component.h"
#include "Entity.h"
#include "SpellComponent.h"

#include "Timer.h"

#ifndef MAGIC_COMPONENT_H
#define MAGIC_COMPONENT_H

struct MagicComponent : public Component {
	MagicComponent(Entity *entity_, int main_spell_id_, int secondary_spell_id_, int cast_speed_);
	MagicComponent(Entity *new_entity, const MagicComponent &rhs);
	MagicComponent *copy(Entity *new_entity) const;
	~MagicComponent();

	void update();

	const int get_type() const { return COMPONENT_MAGIC; }

	void cast_main(float x_, float y_, float x2_, float y2_);
	void cast_secondary(float x_, float y_, float x2_, float y2_);
	void cast_special(float x_, float y_, float x2_, float y2_, int spell_id);

	void set_main(int id);
	void set_secondary(int id);

	int main_spell_id;
	int secondary_spell_id;

	Timer cast_timer;
	bool can_cast;

	Entity *main_spell;
	Entity *secondary_spell;
};

#endif