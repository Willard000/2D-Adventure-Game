#include <vector>

#include "SDL_Rect.h"

#include "PositionComponent.h"

#ifndef PATH_H
#define PATH_H

#define PATH_WIDTH 10
#define PATH_HEIGHT 10

struct Path {
	int x = 0, y = 0;	// where to go
	float dx = 0.0f, dy = 0.0f; // norm vec
};

inline bool path_collision(const Path &path, SDL_Rect rect) {
	rect.x += rect.w / 2;
	rect.y += rect.h / 2;

	if (path.dx > 0) {
		if (path.dy > 0) {
			if (rect.x >= path.x && rect.y >= path.y) {
				return true;
			}
			return false;
		}
		if (path.dy < 0) {
			if (rect.x >= path.x && rect.y <= path.y) {
				return true;
			}
			return false;
		}
		if (path.dy == 0) {
			if (rect.x >= path.x) {
				return true;
			}
			return false;
		}
	}
	else if (path.dx < 0) {
		if (path.dy > 0) {
			if (rect.x <= path.x && rect.y >= path.y) {
				return true;
			}
			return false;
		}
		if (path.dy < 0) {
			if (rect.x <= path.x && rect.y <= path.y) {
				return true;
			}
			return false;
		}
		if (path.dy == 0) {
			if (rect.x <= path.x) {
				return true;
			}
			return false;
		}
	}
	else if (path.dx == 0) {
		if (path.dy > 0) {
			if (rect.y >= path.y) {
				return true;
			}
			return false;
		}
		if (path.dy < 0) {
			if (rect.y <= path.y) {
				return true;
			}
			return false;
		}
	}

	return false;
}

inline void update_path(std::vector<Path> &pathing, PositionComponent *position) {
	if (position && pathing.size() > 0) {
		if (!path_collision(pathing[0], position->rect)) {
			if (pathing[0].dx == 0 && pathing[0].dy == 0) {
				float x = (float)pathing[0].x - (position->rect.x + position->rect.w / 2);
				float y = (float)pathing[0].y - (position->rect.y + position->rect.h / 2);

				float vec_mag = 1.0f / sqrt(pow(x, 2) + pow(y, 2));
				x *= vec_mag * position->speed;
				y *= vec_mag * position->speed;

				pathing[0].dx = x;
				pathing[0].dy = y;
			}
			position->move_freely(pathing[0].dx, pathing[0].dy);
		}
		else {
			pathing.push_back({ pathing[0].x, pathing[0].y, 0.0f, 0.0f });
			pathing.erase(pathing.begin());
		}
	}
}


#endif