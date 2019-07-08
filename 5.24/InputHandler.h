
#include "Entity.h"

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

void move_entity(Entity *entity, int dir);
void move_camera(int dir);
void toggle_camera();
void cast_spell(double x, double y);

#endif