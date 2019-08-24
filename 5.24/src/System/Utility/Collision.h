
#include <SDL.h>

#ifndef COLLISION_H
#define COLLISION_H

// non rotated quads
inline bool collision(const SDL_Rect &a, const SDL_Rect &b) {
	if ((a.x + a.w) <= b.x ||
		a.x >= (b.x + b.w) ||
		(a.y + a.h) <= b.y ||
		a.y >= (b.y + b.h))
		return false;

	return true;
}

inline bool operator!=(const SDL_Rect &a, const SDL_Rect &b) {
	return a.x != b.x || a.y != b.y || a.w != b.w || a.h != b.h;
}

inline bool operator==(const SDL_Rect &a, const SDL_Rect &b) {
	return a.x == b.x && a.y == b.y && a.w == b.w && a.h == b.h;
}

// SAT
inline bool collision(const SDL_Rect &a, const float &a_rot, const SDL_Rect &b, const float &b_rot) {

}

#endif