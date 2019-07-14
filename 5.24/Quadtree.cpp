/*
#include "Quadtree.h"

#include "Entity.h"

#include "Collision.h"


//test
#include "Environment.h"
#include "Window.h"


template <class T>
QuadTree<T>::QuadTree(SDL_Rect rect, Uint8 max_capacity, unsigned int depth) :
	_rect				( rect ),
	_max_capacity		( max_capacity ),
	_depth				( depth ),
	_northeast			( nullptr ),
	_northwest			( nullptr ),
	_southeast			( nullptr ),
	_southwest			( nullptr )
{}

template <class T>
QuadTree<T>::~QuadTree(){
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
		&& _northeast == nullptr) {
		_objects.push_back(Tree_Object(object, position));
		return true;
	}

	if (!_northeast) {
		subdivide();
	}

	if (_northeast->insert(object, position)) return true;
	if (_northwest->insert(object, position)) return true;
	if (_southeast->insert(object, position)) return true;
	if (_southwest->insert(object, position)) return true;

	return false;
}


















/*
template <class T>
T *QuadTree<T>::get_first_collision(const SDL_Rect &position) {
	if (!collision(position, _rect)) {
		return false;
	}

	for (auto it = _objects.begin(); it != _objects.end(); it++) {
		if (collision(position, *(it->second))) {
			return it->first;
		}
	}

	if (!_northeast) {
		return nullptr;
	}

	Entity *entity;
	if (entity = _northeast->get_first_collision(position))	return entity;
	if (entity = _northwest->get_first_collision(position)) return entity;
	if (entity = _southeast->get_first_collision(position)) return entity;
	if (entity = _southwest->get_first_collision(position)) return entity;

	return nullptr;
}
*/


















/*

template <class T>
bool QuadTree<T>::check_collision(const SDL_Rect &position) {
	if (!collision(position, _rect)) {
		return false;
	}

	for (auto it = _objects.begin(); it != _objects.end(); it++) {
		if (collision(position, *(it->second))) {
			return true;
		}
	}

	if (!_northeast)
		return false;

	if (_northeast->check_collision(position)) return true;
	if (_northwest->check_collision(position)) return true;
	if (_southeast->check_collision(position)) return true;
	if (_southwest->check_collision(position)) return true;

	return false;
}

template <class T>
void QuadTree<T>::clear() {
	if (_northeast) {
		_northeast->clear();
		delete _northeast;
		_northeast = nullptr;
	}
	if (_northwest) {
		_northwest->clear();
		delete _northwest;
		_northwest = nullptr;
	}
	if (_southeast) {
		_southeast->clear();
		delete _southeast;
		_southeast = nullptr;
	}
	if (_southwest) {
		_southwest->clear();
		delete _southwest;
		_southwest = nullptr;
	}

	_objects.clear();
}

template <class T>
void QuadTree<T>::set_new_rect(SDL_Rect rect) {
	_rect = rect;

	// re build tree???
}

template <class T>
void QuadTree<T>::subdivide() {
	int width = _rect.w / 2;
	int height = _rect.h / 2;
	_northeast = new QuadTree({ _rect.x, _rect.y, width, height }, _depth + 1, _max_capacity);
	_northwest = new QuadTree({ _rect.x + width, _rect.y, width, height }, _depth + 1, _max_capacity);
	_southeast = new QuadTree({ _rect.x, _rect.y + height, width, height }, _depth + 1, _max_capacity);
	_southwest = new QuadTree({ _rect.x + width, _rect.y + height, width, height }, _depth + 1, _max_capacity);

	for (auto it = _objects.begin(); it != _objects.end(); it++) {
		insert(it->first, it->second);
	}

	_objects.clear();
}

#include <iostream>
template <class T>
void QuadTree<T>::print() {
	std::cout << "D: " << _depth << std::endl;
	for (auto it = _objects.begin(); it != _objects.end(); it++) {
//		std::cout << "E: " << (*it)->entity->get_id() << " " << (*it)->entity->get_type() << " " << (*it)->entity->get_type_id() << std::endl;
//		std::cout << it->second->x << " " << (*it)->y << " " << (*it)->w << " " << (*it)->h << std::endl;
	}

	if (_northeast) {
		std::cout << "North East: " << std::endl;
		_northeast->print();
	}
	if (_northwest) {
		std::cout << "North West: " << std::endl;
		_northwest->print();
	}
	if (_southeast) {
		std::cout << "South East: " << std::endl;
		_southeast->print();
	}
	if (_southwest) {
		std::cout << "South West: " << std::endl;
		_southwest->print();
	}
}

template <class T>
void QuadTree<T>::draw(SDL_Color color) {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_rect(_rect, color, DRAW_RECT_CAMERA);

	if (_northeast) _northeast->draw({ 0, 200, 0, 50 });
	if (_northwest) _northwest->draw({ 0, 0, 200, 50 });
	if (_southeast) _southeast->draw({ 200, 0, 200, 50 });
	if (_southwest) _southwest->draw({ 0, 200, 200, 50 });
}

*/