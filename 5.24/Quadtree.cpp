#include "Quadtree.h"

template <class T>
QuadTree<T>::QuadTree(SDL_Rect rect, int max_size) :
	_rect				( rect ),
	_size				( 0 ),
	_max_size			( max_size ),
	_tree				( nullptr )
{}

template <class T>
QuadTree<T>::~QuadTree(){

}

template <class T>
void QuadTree<T>::push_back(T *obj, SDL_Rect rect) {
	if (_size == _max_size) {
		if (!_tree) {
			_tree = new QuadTree[4];
		}

		// Max Depth?

		if (rect.x < _rect.x + _rect.w / 2) {
			_tree[0].push_back(obj, rect);
		}
		else if (rect.x >= _rect.x + _rect.w / 2) {
			_tree[1].push_back(obj, rect);
		}
		else if (rect.y < _rect.y + _rect.h / 2) {
			_tree[2].push_bacK(obj, rect);
		}
		else if (rect.y >= _rect.y + _rect.h / 2) {
			_tree[3].push_back(obj, rect);
		}
	}
}