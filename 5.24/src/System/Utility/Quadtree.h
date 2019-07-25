#include <SDL.h>
#include <vector>

#include <iostream>

#include "Collision.h"

#ifndef QUADTREE_H
#define QUADTREE_H

template <class T>
class QuadTree {
public:
	// T - object attached to rect
	// SDL_Rect - rect to use in tree
	typedef std::pair<T, SDL_Rect *> Tree_Object;

	QuadTree(SDL_Rect rect, Uint8 max_capacity, int depth = 0);

	~QuadTree();

	bool insert(T object, SDL_Rect *position = nullptr);

	T *get_first_collision(const SDL_Rect &position);

	bool check_collision(const SDL_Rect &position);

	void clear();

	void set_new_rect(SDL_Rect rect);

	void draw(std::vector<std::pair<SDL_Rect *, SDL_Color>> &rects, SDL_Color color = { 200, 0, 0, 50 });

	int size();
private:
	void subdivide();
private:
	SDL_Rect _rect;
	Uint8 _max_capacity;
	int _depth;

	std::vector<Tree_Object> _objects;

	QuadTree *_northeast;
	QuadTree *_northwest;
	QuadTree *_southeast;
	QuadTree *_southwest;
};

template <class T>
QuadTree<T>::QuadTree(SDL_Rect rect, Uint8 max_capacity, int depth) :
	_rect			( rect ),
	_max_capacity	( max_capacity ),
	_depth			( depth ),
	_northeast		( nullptr ),
	_northwest		( nullptr ),
	_southeast		( nullptr ),
	_southwest		( nullptr )
{}

template <class T>
QuadTree<T>::~QuadTree() {
	clear();
}

template <class T>
bool QuadTree<T>::insert(T object, SDL_Rect *position) {
	if (!position) {
		return false;
	}

	if (!collision(*position, _rect)) {
		return false;
	}

	if (_objects.size() < _max_capacity
		&& !_northeast) {
		_objects.push_back(Tree_Object(object, position));
		return true;
	}

	if (!_northeast) {
		subdivide();
	}

	return
		_northeast->insert(object, position) +
		_northwest->insert(object, position) +
		_southeast->insert(object, position) +
		_southwest->insert(object, position);
}

template <class T>
T *QuadTree<T>::get_first_collision(const SDL_Rect &position) {
	if (!collision(position, _rect))
		return false;

	if (!_northeast) {
		for (auto it = _objects.begin(); it != _objects.end(); ++it) {
			if (collision(position, *(it->second))) {
				if (position != *(it->second)) {
					return &(it->first);
				}
			}
		}
		return nullptr;
	}

	T *object = nullptr;
	if (object = _northeast->get_first_collision(position))	return object;
	if (object = _northwest->get_first_collision(position)) return object;
	if (object = _southeast->get_first_collision(position)) return object;
	if (object = _southwest->get_first_collision(position)) return object;

	return nullptr;
}

template <class T>
bool QuadTree<T>::check_collision(const SDL_Rect &position) {
	if (!collision(position, _rect))
		return false;

	if (!_northeast) {
		for (auto it = _objects.begin(); it != _objects.end(); ++it) {
			if (collision(position, *(it->second))) {
				if (position != *(it->second)) {
					return true;
				}
			}
		}
		return false;
	}

	return
		_northeast->check_collision(position) ||
		_northwest->check_collision(position) ||
		_southeast->check_collision(position) ||
		_southwest->check_collision(position);
}

template <class T>
void QuadTree<T>::clear() {
	if (_northeast) {
		_northeast->clear();	 _northwest->clear();
		_southeast->clear();	 _southwest->clear();
		delete _northeast;		 _northeast = nullptr;
		delete _northwest;	 	 _northwest = nullptr;
		delete _southeast;	 	 _southeast = nullptr;
		delete _southwest;		 _southwest = nullptr;
	}

	_objects.clear();
}

template <class T>
void QuadTree<T>::set_new_rect(SDL_Rect rect) {
	_rect = rect;

	// re build tree???
}

template <class T>
void QuadTree<T>::draw(std::vector<std::pair<SDL_Rect *, SDL_Color>> &rects, SDL_Color color) {
	rects.push_back(std::pair<SDL_Rect *, SDL_Color>(&_rect, color));

	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		rects.push_back(std::pair<SDL_Rect *, SDL_Color>(it->second, { 0, 0, 255, 100 }));
	}

	if (!_northeast)
		return;

	_northeast->draw(rects, { 0, 200, 0, 50 });
	_northwest->draw(rects, { 0, 0, 200, 50 });
	_southeast->draw(rects, { 200, 0, 200, 50 });
	_southwest->draw(rects, { 0, 200, 200, 50 });
}

template <class T>
int QuadTree<T>::size() {
	if (!_northeast) {
		return _objects.size();
	}

	return _northeast->size() + _northwest->size() + _southeast->size() + _southwest->size();
}

template <class T>
void QuadTree<T>::subdivide() {
	int width = _rect.w / 2;
	int height = _rect.h / 2;
	_northeast = new QuadTree({ _rect.x + width, _rect.y, width, height }, _depth + 1, _max_capacity);
	_northwest = new QuadTree({ _rect.x, _rect.y, width, height }, _depth + 1, _max_capacity);
	_southeast = new QuadTree({ _rect.x + width, _rect.y + height, width, height }, _depth + 1, _max_capacity);
	_southwest = new QuadTree({ _rect.x, _rect.y + height, width, height }, _depth + 1, _max_capacity);

	for (auto it = _objects.begin(); it != _objects.end(); ++it) {
		insert(it->first, it->second);
	}

	_objects.clear();
}
#endif