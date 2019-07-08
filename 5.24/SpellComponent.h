#include "Component.h"

#include <SDL.h>

#ifndef SPELL_COMPONENT_H
#define SPELL_COMPONENT_H

struct SpellComponent : public Component {
	SpellComponent(Entity *entity_, double max_dis_, double speed_);

	virtual void update();

	void cast(double x_, double y_);

	double dis, max_dis;
	double speed;
	double dx, dy;
	double des_x, des_y;

};

#endif
