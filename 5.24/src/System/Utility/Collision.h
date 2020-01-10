
#include <SDL.h>
#include <array>

#ifndef COLLISION_H
#define COLLISION_H

struct Vertex {
	double x = 0, y = 0;
};

struct Projection {
	double min = 0, max = 0;
};

std::array<Vertex, 4> make_vertices(const SDL_Rect &rect, const double &rot);

// SAT
bool sat_collision(const SDL_Rect &, const SDL_Rect &b, const double &a_rot, const double &b_rot);

// non rotated quads
inline bool collision(const SDL_Rect &a, const SDL_Rect &b, const double &a_rot = 0, const double &b_rot = 0) {
	if (a_rot != 0 || b_rot != 0) {
		return sat_collision(a, b, a_rot, b_rot);
	}

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

#endif