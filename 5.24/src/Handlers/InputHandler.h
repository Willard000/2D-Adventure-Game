
#include "Entity.h"

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

void move_entity(Entity *entity, int dir);
void move_camera(int dir);
void toggle_camera();
void cast_spell(float x, float y, bool main_spell = true);
void pickup_item();
void interact();
void open_shop();
void execute_command();

#endif