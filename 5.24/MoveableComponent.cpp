#include "MoveableComponent.h"

MoveableComponent::MoveableComponent(Entity *entity_, double x_, double y_, int w_, int h_, double speed_) 
	: PositionComponent(entity_, x_, y_, w_, h_) {
		speed = speed_;
}

void MoveableComponent::move(int dir, double time) {
	if (dir == MoveEvent::MOVE_UP) {
		pos_y -= speed * time;
	}
	else if (dir == MoveEvent::MOVE_DOWN) {
		pos_y += speed * time;
	}
	else if (dir == MoveEvent::MOVE_LEFT) {
		pos_x -= speed * time;
	}
	else if (dir == MoveEvent::MOVE_RIGHT) {
		pos_x += speed * time;
	}

	to_SDL_Rect();

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir;
	}
}

void MoveableComponent::move(int dir, double dis, double time) {
	if (dir == MoveEvent::MOVE_UP) {
		pos_y -= dis * time;
	}
	else if (dir == MoveEvent::MOVE_DOWN) {
		pos_y += dis * time;
	}
	else if (dir == MoveEvent::MOVE_LEFT) {
		pos_x -= dis * time;
	}
	else if (dir == MoveEvent::MOVE_RIGHT) {
		pos_x += dis * time;
	}

	to_SDL_Rect();

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir;
	}
}