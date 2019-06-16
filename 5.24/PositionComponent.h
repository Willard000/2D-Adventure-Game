#include <SDL.h>

#include "Component.h"

#ifndef POSITION_COMPONENT_H
#define POSITION_COMPONENT_H

struct PositionComponent : public Component {
public:
	PositionComponent(Entity *entity, double x, double y, int w, int h, double speed);

	virtual void update() {}

	void move(int dir);
	void move(int dir, double dis);

	void scale(double val);
	void rotate(double val);

	double m_pos_x, m_pos_y;
	double m_speed;
	double m_scale, m_rotation;
	SDL_Rect m_rect;
};

#endif