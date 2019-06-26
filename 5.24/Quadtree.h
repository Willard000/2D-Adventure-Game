#include <SDL.h>
#include <vector>

#ifndef QUADTREE_H
#define QUADTREE_H

template <class T>
class QuadTree {
public:
	QuadTree(SDL_Rect rect, int max_size);
	~QuadTree();

	void push_back(T *obj, SDL_Rect rect);
private:
	SDL_Rect _rect;
	std::vector<T*> _objects;
	QuadTree *_tree;
	int _size;
	int _max_size;
};

#endif