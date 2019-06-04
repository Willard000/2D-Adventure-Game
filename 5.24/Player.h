#include <string>

#include "Entity.h"

#ifndef PLAYER_H
#define PLAYER_H

class Player : public Entity {
public:
	Player(int type_id) : Entity(TYPE_PLAYER, type_id) {};
private:
};

#endif