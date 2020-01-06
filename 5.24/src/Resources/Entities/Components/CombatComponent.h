#include <vector>

#include "Component.h"

#include "EnemyComponent.h"

#include "Timer.h"

#ifndef COMBAT_COMPONENT_H
#define COMBAT_COMPONENT_H

struct Combat_Info {
	int damage = 0, armor = 0, drain = 0, luck = 0, exp = 0;
};

struct Buff_Info {
	Timer timer;
	int health = 0, mana = 0, damage = 0, armor = 0, speed = 0, luck = 0;
	int icon_item_id = 0;
};

struct CombatComponent : public Component {
	CombatComponent(Entity *entity_, int max_health_, int max_mana_, int damage_, int armor_, int hps_, int mps_, int drain_, int luck_, int exp_, int combat_time);
	CombatComponent(Entity *new_entity, const CombatComponent &rhs);
	CombatComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const ;

	void apply_damage(Combat_Info &attacker_info, const SDL_Color &color);

	void draw_health();
	void draw_buffs();

	void start_combat();

	void death();

	void drop_items(EnemyComponent *enemy);

	void add_buff(Buff_Info buff, bool new_buff = true);
	void remove_buff(Buff_Info &buff);

	int health, max_health;
	int mana, max_mana;
	int damage, armor;
	int hps, mps;
	int drain;
	int luck;
	int exp;

	bool in_combat;
	Timer combat_timer;
	Timer hps_mps_timer;

	std::vector<Buff_Info> buffs;
};

#endif