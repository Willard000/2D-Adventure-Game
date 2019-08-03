#include "SpriteComponent.h"

#include "Globals.h"

#include "EffectComponent.h"
#include "Entity.h"

SpriteComponent::SpriteComponent(Entity *entity_, int w_, int h_, int time_, int cast_time_) : 
	Component		( entity_ ),
	pos				( { 0, 0, w_, h_ } ),
	is_update		( false ),
	frame			( 0 ),
	ani				( NULL ),	
	ani_prev		( NULL ),
	dir				( NULL ),		
	dir_prev		( NULL ),
	time			( time_ ),
	cast_timer		( cast_time_ )
{}

SpriteComponent::SpriteComponent(Entity *new_entity, const SpriteComponent &rhs) :
	Component		( new_entity ),
	pos				( rhs.pos ),
	is_update		( rhs.is_update ),
	frame			( rhs.frame ),
	ani				( rhs.ani ),
	ani_prev		( rhs.ani_prev ),
	dir				( rhs.dir ),
	dir_prev		( rhs.dir_prev ),
	time			( rhs.time ),
	cast_timer		( rhs.cast_timer )
{}

SpriteComponent *SpriteComponent::copy(Entity *new_entity) const {
	return new SpriteComponent(new_entity, *this);
}

void SpriteComponent::update() {
	if (!is_update) {
		is_update = time.update();
	}
}

void SpriteComponent::update(Sprite *img) {
	if ((dir == dir_prev) && 
		(ani == ani_prev) && 
		(ani != CAST) &&
		!is_update) {
		return;
	}

	if (ani == NULL) {
		frame = img->idle;
	}
	else if (ani == MOVE) {
		frame++;
		if (dir == NULL) {
			frame = img->idle;
		}
		else if (dir == MOVE_UP) {
			if (frame < img->min_up || frame > img->max_up) {
				frame = img->min_up;
			}
		}
		else if (dir == MOVE_DOWN) {
			if (frame < img->min_down || frame > img->max_down) {
				frame = img->min_down;
			}
		}
		else if (dir == MOVE_LEFT || dir == MOVE_UPLEFT|| dir == MOVE_UPRIGHT) {
			if (frame < img->min_left || frame > img->max_left) {
				frame = img->min_left;
			}
		}
		else if (dir == MOVE_RIGHT || dir == MOVE_DOWNLEFT|| dir == MOVE_DOWNRIGHT) {
			if (frame < img->min_right || frame > img->max_right) {
				frame = img->min_right;
			}
		}

		dir_prev = dir;
	}
	else if (ani == IDLE) {
		frame++;
		if (frame < img->min_idle || frame > img->max_idle) {
			frame = img->min_idle;
		}
	}
	else if (ani == CAST && !cast_timer.update()) {
		frame = img->cast;

		pos.x = img->get_frame(frame).x;
		pos.y = img->get_frame(frame).y;

		ani_prev = ani;
		is_update = false;
		return;
	}

	ani_prev = ani;
	ani = NULL;
	is_update = false;

	pos.x = img->get_frame(frame).x;
	pos.y = img->get_frame(frame).y;
}