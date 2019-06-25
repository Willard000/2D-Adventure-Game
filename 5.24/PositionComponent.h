#include <SDL.h>

#include "Component.h"

#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

struct PositionComponent : public Component {
public:
	PositionComponent(Entity *entity_, double x_, double y_, int w_, int h_, double speed_);

	virtual void update() {}

	void move(int dir_);
	void move(int dir_, double dis_);

	void scale(float factor_);
	void setScale(float factor_);
	void rotate(double angle_);
	void setRotation(double angle_);

	double pos_x, pos_y;
	double speed;
	float scale_f;
	double rotation;
	SDL_Rect rect;
	int base_width;
	int base_height;
};

#endif