#include "SpriteComponent.h"

#include "Event.h"

SpriteComponent::SpriteComponent(Entity *entity, int w, int h, int time) : 
	Component		(entity),
	m_pos			( { 0, 0, 32, 32 } )
{
	m_time.set(time);
};

void SpriteComponent::update() {
	if (!m_is_update) {
		m_is_update = m_time.update();
	}
}

void SpriteComponent::update(Sprite *img) {
	if ((m_dir == m_dir_prev) && 
		(m_ani == m_ani_prev) && 
		(m_ani != CAST)       &&
		!m_is_update) {
		return;
	}

	if (m_ani == NULL) {
		m_frame = 0;
		m_is_update = false;
	}
	else if (m_ani == MOVE) {
		m_frame++;
		m_is_update = false;
		if (m_dir == NULL) {
			m_frame = 0;
		}
		else if (m_dir == Event::UP) {
			if (m_frame < img->m_min_up || m_frame > img->m_max_up) {
				m_frame = img->m_min_up;
			}
		}
		else if (m_dir == Event::DOWN) {
			if (m_frame < img->m_min_down || m_frame > img->m_max_down) {
				m_frame = img->m_min_down;
			}
		}
		else if (m_dir == Event::LEFT || m_dir == Event::UPLEFT|| m_dir == Event::DOWNLEFT) {
			if (m_frame < img->m_min_left || m_frame > img->m_max_left) {
				m_frame = img->m_min_left;
			}
		}
		else if (m_dir == Event::RIGHT || m_dir == Event::DOWNLEFT || m_dir == Event::DOWNRIGHT) {
			if (m_frame < img->m_min_right || m_frame > img->m_max_right) {
				m_frame = img->m_min_right;
			}
		}

		m_dir_prev = m_dir;
	}

	m_ani_prev = m_ani;
	m_ani = NULL;

	const Sprite::Frame sprite_frame = img->get_frame(m_frame);
	m_pos.x = sprite_frame.x;
	m_pos.y = sprite_frame.y;
}