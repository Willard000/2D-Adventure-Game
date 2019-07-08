#include "PositionComponent.h"

#include "Entity.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Clock.h"

#include "Globals.h"

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

void PositionComponent::update() {}

void PositionComponent::move(int dir_, double dis_) {
	double distance = dis_ == 0 ? speed : dis_;
	double prev_pos_x = pos_x;
	double prev_pos_y = pos_y;

	if (dir_ == MOVE_UP) {
		pos_y -= distance * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == MOVE_DOWN) {
		pos_y += distance * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == MOVE_LEFT) {
		pos_x -= distance * Environment::get().get_clock()->get_time();
	}
	else if (dir_ == MOVE_RIGHT) {
		pos_x += distance * Environment::get().get_clock()->get_time();
	}

	SpriteComponent *sprite = GetSprite(entity);
	if (sprite != nullptr) {
		sprite->ani = MOVE;
		sprite->dir = dir_;
	}

	rect.x = int(pos_x);
	rect.y = int(pos_y);

	if (Environment::get().get_resource_manager()->get_map()->solid_collision(rect)) {
		set(prev_pos_x, prev_pos_y);
	}
}

void PositionComponent::set(double x, double y) {
	pos_x = x;
	pos_y = y;
	rect.x = (int)pos_x;
	rect.y = (int)pos_y;
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