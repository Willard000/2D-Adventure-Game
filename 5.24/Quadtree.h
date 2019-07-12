#include <SDL.h>
#include <vector>

#include "PositionComponent.h"

#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTree {
public:
	QuadTree(SDL_Rect rect, Uint8 max_capacity, unsigned int depth = 0);
	~QuadTree();

	bool insert(SDL_Rect *position);
	
	bool find(const SDL_Rect &position);

	void clear();

	void set_new_rect(SDL_Rect rect);

	void print();

	void draw(SDL_Color color = { 200, 0, 0, 50 });
private:
	void subdivide();
private:
	SDL_Rect _rect;
	Uint8 _max_capacity;
	int _depth;

	std::vector<SDL_Rect *> _positions;

	QuadTree *_northeast;
	QuadTree *_northwest;
	QuadTree *_southeast;
	QuadTree *_southwest;
};

#endif