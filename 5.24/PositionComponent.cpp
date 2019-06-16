#include "PositionComponent.h"

#include "Event.h"

#include "Entity.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "Clock.h"

PositionComponent::PositionComponent(Entity *entity, double x, double y, int w, int h, double speed)
	: Component(entity) {
	m_pos_x = x;
	m_pos_y = y;
	m_speed = speed;
	m_scale = 1.0;
	m_rotation = 0.0;
	m_rect = { int(m_pos_x), int(m_pos_y), w, h };
}

void PositionComponent::move(int dir) {
	if (dir == Event::UP) {
		m_pos_y -= m_speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::DOWN) {
		m_pos_y += m_speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::LEFT) {
		m_pos_x -= m_speed * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::RIGHT) {
		m_pos_x += m_speed * Environment::get().getClock()->getTime();
	}

	m_rect.x = int(m_pos_x);
	m_rect.y = int(m_pos_y);

	SpriteComponent *sprite = GetSprite(m_entity);
	if (sprite != nullptr) {
		sprite->m_ani = MOVE;
		sprite->m_dir = dir;
	}
}

void PositionComponent::move(int dir, double dis) {
	if (dir == Event::UP) {
		m_pos_y -= dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::DOWN) {
		m_pos_y += dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::LEFT) {
		m_pos_x -= dis * Environment::get().getClock()->getTime();
	}
	else if (dir == Event::RIGHT) {
		m_pos_x += dis * Environment::get().getClock()->getTime();
	}

	m_rect.x = int(m_pos_x);
	m_rect.y = int(m_pos_y);

	SpriteComponent *sprite = GetSprite(m_entity);
	if (sprite != nullptr) {
		sprite->m_ani = MOVE;
		sprite->m_dir = dir;
	}
}

void PositionComponent::scale(double val) {
	m_scale += val;
}

void PositionComponent::rotate(double val) {
	m_rotation += val;
}