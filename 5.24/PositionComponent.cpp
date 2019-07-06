#include "PositionComponent.h"

#include "Event.h"

#include "Entity.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Clock.h"

PositionComponent::PositionComponent(Entity *entity_, double x_, double y_, int w_, int h_, double speed_) :
	Component		( entity_ ),
	pos_x			( x_ ),
	pos_y			( y_ ),
	speed			( speed_ ),
	scale_f			( 1.0 ),
	rotation		( 0.0 ),
	rect			( { int(pos_x), int(pos_y), w_, h_ } ),
	base_width      ( w_ ),
	base_height     ( h_ )
{}

void PositionComponent::update() {

}

void PositionComponent::move(int dir_) {
	double prev_pos_x = pos_x;
	double prev_pos_y = pos_y;

	if (dir_ == Event::UP) {
		pos_y -= speed * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::DOWN) {
		pos_y += speed * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::LEFT) {
		pos_x -= speed * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::RIGHT) {
		pos_x += speed * Environment::get().get_clock()->get_time();
	}

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir_;
	}

	rect.x = int(pos_x);
	rect.y = int(pos_y);

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(rect)) {
		pos_x = prev_pos_x;
		pos_y = prev_pos_y;
		rect.x = int(pos_x);
		rect.y = int(pos_y);
	}
}

void PositionComponent::move(int dir_, double dis_) {
	double prev_pos_x = pos_x;
	double prev_pos_y = pos_y;

	if (dir_ == Event::UP) {
		pos_y -= dis_ * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::DOWN) {
		pos_y += dis_ * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::LEFT) {
		pos_x -= dis_ * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == Event::RIGHT) {
		pos_x += dis_ * Environment::get().get_clock()->get_time();
	}

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir_;
	}

	rect.x = int(pos_x);
	rect.y = int(pos_y);

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(rect)) {
		pos_x = prev_pos_x;
		pos_y = prev_pos_y;
		rect.x = int(pos_x);
		rect.y = int(pos_y);
	}
}

void PositionComponent::scale(float factor_) {
	scale_f += factor_;
	rect.w = int(base_width * scale_f);
	rect.h = int(base_height * scale_f);
}

void PositionComponent::set_scale(float factor_) {
	scale_f = factor_;
	rect.w = int(base_width * scale_f);
	rect.h = int(base_height * scale_f);
}

void PositionComponent::rotate(double angle_) {
	rotation += angle_;
}

void PositionComponent::set_rotation(double angle_) {
	rotation = angle_;
}