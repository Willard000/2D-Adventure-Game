#include "PositionComponent.h"

PositionComponent::PositionComponent(Entity *entity_, double x_, double y_, int w_, int h_)
	: Component(entity_) {
	pos_x = x_;
	pos_y = y_;
	position = { int(pos_x), int(pos_y), w_, h_ };
}

void PositionComponent::to_SDL_Rect() {
	position.x = int(pos_x);
	position.y = int(pos_y);
}