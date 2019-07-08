#include "SpellComponent.h"

#include "Entity.h"
#include "PositionComponent.h"

SpellComponent::SpellComponent(Entity *entity_, double max_dis_, double speed_) :
	Component	( entity_ ),
	dis			( 0 ),
	max_dis		( max_dis_ ),
	speed		( speed_ ),
	dx			( 0 ),
	dy			( 0 ),
	des_x		( 0 ),
	des_y		( 0 )
{}

void SpellComponent::update() {
	if (PositionComponent *position = GetPosition(entity)) {
		position->set(position->pos_x + dx, position->pos_y + dy);
	}

}

void SpellComponent::cast(double x_, double y_) {
	PositionComponent *position = GetPosition(entity);
	if (!position) {
		return;
	}

	double adj = (x_ - position->pos_x) - (position->rect.w / 2);
	double opp = (y_ - position->pos_y) - (position->rect.h / 2);

	if (abs(opp) > abs(adj)) {
		dx = abs(adj) / abs(opp);
		dy = 2 - dx;
	}
	else {
		dy = abs(opp) / abs(adj);
		dx = 2 - dy;
	}

	if (adj < 0) {
		dx = -dx;
	}

	if (opp < 0) {
		dy = -dy;
	}

	dx *= speed;
	dy *= speed;
}