#include "Quadtree.h"

#include "Entity.h"

#include "Collision.h"


//test
#include "Environment.h"
#include "Window.h"


QuadTree::QuadTree(SDL_Rect rect, Uint8 max_capacity, unsigned int depth) :
	_rect				( rect ),
	_max_capacity		( max_capacity ),
	_depth				( depth ),
	_northeast			( nullptr ),
	_northwest			( nullptr ),
	_southeast			( nullptr ),
	_southwest			( nullptr )
{}


QuadTree::~QuadTree(){
	clear();
}

bool QuadTree::insert(SDL_Rect *position) {
	if (!position) {
		return false;
	}

	if (!collision(*position, _rect)) {
		return false;
	}

	if (_positions.size() < _max_capacity
		&& _northeast == nullptr) {
		_positions.push_back(position);
		return true;
	}

	if (_northeast == nullptr) {
		subdivide();
	}

	if (_northeast->insert(position)) return true;
	if (_northwest->insert(position)) return true;
	if (_southeast->insert(position)) return true;
	if (_southwest->insert(position)) return true;

	return false;
}

bool QuadTree::find(const SDL_Rect &position) {
	if (!collision(position, _rect)) {
		return false;
	}

	for (auto it = _positions.begin(); it != _positions.end(); it++) {
		if (collision(position, *(*it))) {
			return true;
		}
	}

	if (_northeast && _northeast->find(position)) return true;
	if (_northwest && _northwest->find(position)) return true;
	if (_southeast && _southeast->find(position)) return true;
	if (_southwest && _southwest->find(position)) return true;

	return false;
}

void QuadTree::clear() {
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

	_positions.clear();
}

void QuadTree::set_new_rect(SDL_Rect rect) {
	_rect = rect;

	// re build tree???
}

void QuadTree::subdivide() {
	int width = _rect.w / 2;
	int height = _rect.h / 2;
	_northeast = new QuadTree({ _rect.x, _rect.y, width, height }, _depth + 1, _max_capacity);
	_northwest = new QuadTree({ _rect.x + width, _rect.y, width, height }, _depth + 1, _max_capacity);
	_southeast = new QuadTree({ _rect.x, _rect.y + height, width, height }, _depth + 1, _max_capacity);
	_southwest = new QuadTree({ _rect.x + width, _rect.y + height, width, height }, _depth + 1, _max_capacity);

	for (auto it = _positions.begin(); it != _positions.end(); it++) {
		insert(*it);
	}

	_positions.clear();
}

#include <iostream>
void QuadTree::print() {
	std::cout << "D: " << _depth << std::endl;
	for (auto it = _positions.begin(); it != _positions.end(); it++) {
//		std::cout << "E: " << (*it)->entity->get_id() << " " << (*it)->entity->get_type() << " " << (*it)->entity->get_type_id() << std::endl;
		std::cout << (*it)->x << " " << (*it)->y << " " << (*it)->w << " " << (*it)->h << std::endl;
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

void QuadTree::draw(SDL_Color color) {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->draw_rect(_rect, color, DRAW_RECT_CAMERA);

	if (_northeast) _northeast->draw({ 0, 200, 0, 50 });
	if (_northwest) _northwest->draw({ 0, 0, 200, 50 });
	if (_southeast) _southeast->draw({ 200, 0, 200, 50 });
	if (_southwest) _southwest->draw({ 0, 200, 200, 50 });
}