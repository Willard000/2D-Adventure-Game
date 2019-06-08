#include "SpriteComponent.h"

SpriteComponent::SpriteComponent(Entity *entity_, int w, int h, int time_)
	: Component(entity_) {
		pos.w = w;
		pos.h = h;
		time.set(time_);
};

void SpriteComponent::update() {
	if (!is_update) {
		is_update = time.update();
	}
}

void SpriteComponent::update(Sprite *img) {
	if ((dir == dir_prev) && (ani == ani_prev) && (ani != CAST) && !is_update) {
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
		else if (dir == MoveEvent::MOVE_UP) {
			if (frame < img->min_up || frame > img->max_up) {
				frame = img->min_up;
			}
		}
		else if (dir == MoveEvent::MOVE_DOWN) {
			if (frame < img->min_down || frame > img->max_down) {
				frame = img->min_down;
			}
		}
		else if (dir == MoveEvent::MOVE_LEFT || dir == MoveEvent::MOVE_UPLEFT|| dir == MoveEvent::MOVE_DOWNLEFT) {
			if (frame < img->min_left || frame > img->max_left) {
				frame = img->min_left;
			}
		}
		else if (dir == MoveEvent::MOVE_RIGHT || dir == MoveEvent::MOVE_DOWNLEFT || dir == MoveEvent::MOVE_DOWNRIGHT) {
			if (frame < img->min_right || frame > img->max_right) {
				frame = img->min_right;
			}
		}

		dir_prev = dir;
	}

	ani_prev = ani;
	ani = NULL;

	if (frame > Sprite::SPRITE_SHEET_WIDTH - 1) {
		pos.x = (frame % Sprite::SPRITE_SHEET_WIDTH * img->rect.w);
		pos.y = (frame / Sprite::SPRITE_SHEET_WIDTH * img->rect.h);
	}
	else if (frame > 0) {
		pos.x = frame * img->rect.w;
	}
	else {
		pos.x = 0;
		pos.y = 0;
	}
}