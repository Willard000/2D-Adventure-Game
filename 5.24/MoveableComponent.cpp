#include "MoveableComponent.h"

#include "Environment.h"
#include "Clock.h"

#include "Event.h"
#include "SpriteComponent.h"

MoveableComponent::MoveableComponent(Entity *entity_, double x_, double y_, int w_, int h_, double speed_) 
	: PositionComponent(entity_, x_, y_, w_, h_) {
		speed = speed_;
}

void MoveableComponent::move(int dir) {
	if (dir == Event::UP) {
		pos_y -= speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::DOWN) {
		pos_y += speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::LEFT) {
		pos_x -= speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::RIGHT) {
		pos_x += speed * Environment::get().getClock()->getTime();
	}

	to_SDL_Rect();

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir;
	}
}

void MoveableComponent::move(int dir, double dis) {
	if (dir == Event::UP) {
		pos_y -= dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::DOWN) {
		pos_y += dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::LEFT) {
		pos_x -= dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::RIGHT) {
		pos_x += dis * Environment::get().getClock()->getTime();
	}

	to_SDL_Rect();

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir;
	}
}