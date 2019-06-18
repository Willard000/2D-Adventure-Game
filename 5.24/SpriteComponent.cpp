#include "SpriteComponent.h"

#include "Event.h"

SpriteComponent::SpriteComponent(Entity *entity_, int w_, int h_, int time_) : 
	Component		( entity_ ),
	pos				( { 0, 0, 32, 32 } )
{
	time.set		( time_ );
};

void SpriteComponent::update() {
	if (!is_update) {
		is_update = time.update();
	}
}

void SpriteComponent::update(Sprite *img) {
	if ((dir == dir_prev) && 
		(ani == ani_prev) && 
		(ani != CAST)       &&
		!is_update) {
		return;
	}

	if (ani == NULL) {
		frame = 0;
		is_update = false;
	}
	else if (ani == MOVE) {
		frame++;
		is_update = false;
		if (dir == NULL) {
			frame = 0;
		}
		else if (dir == Event::UP) {
			if (frame < img->min_up || frame > img->max_up) {
				frame = img->min_up;
			}
		}
		else if (dir == Event::DOWN) {
			if (frame < img->min_down || frame > img->max_down) {
				frame = img->min_down;
			}
		}
		else if (dir == Event::LEFT || dir == Event::UPLEFT|| dir == Event::DOWNLEFT) {
			if (frame < img->min_left || frame > img->max_left) {
				frame = img->min_left;
			}
		}
		else if (dir == Event::RIGHT || dir == Event::DOWNLEFT || dir == Event::DOWNRIGHT) {
			if (frame < img->min_right || frame > img->max_right) {
				frame = img->min_right;
			}
		}

		dir_prev = dir;
	}

	ani_prev = ani;
	ani = NULL;

	pos.x = img->get_frame(frame).x;
	pos.y = img->get_frame(frame).y;
}