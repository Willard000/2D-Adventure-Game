
#include <SDL.h>

#ifndef COLLISION_H
#define COLLISION_H

bool collision(const SDL_Rect &a, const SDL_Rect &b);

bool spell_collisions(const SDL_Rect &pos);

#endif