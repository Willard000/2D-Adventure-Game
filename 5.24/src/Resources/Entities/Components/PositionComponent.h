#include <SDL.h>

#include "Component.h"

#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

struct PositionComponent : public Component {
public:
	PositionComponent(Entity *entity_, float x_, float y_, int w_, int h_, float speed_);
	PositionComponent(Entity *new_entity, const PositionComponent &rhs);
	PositionComponent *copy(Entity *new_entity) const;

	void update();

	const int get_type() const { return COMPONENT_POSITION; }

	void move(int dir_, float dis_ = 0.0f);

	void set(float x, float y);

	void scale(float factor_);
	void set_scale(float factor_);
	void rotate(double angle_);
	void set_rotation(double angle_);

	float pos_x, pos_y;
	float speed;
	float scale_f;
	double rotation;
	SDL_Rect rect;
	int base_width;
	int base_height;
};

#endif