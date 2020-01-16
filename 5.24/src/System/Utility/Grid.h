#include <iostream>
#include <SDL.h>
#include <vector>

#include "Collision.h"

// Objects that are too big and fit into more then 4 cells, do not work

#ifndef GRID_H
#define GRID_H

template <class T>
class Grid {
public:
	Grid();
	Grid(int cell_width, int cell_height, int row_width, int height);
	~Grid();

	// inserts an object into the grid at location pos
	// returns the amount of times the object was inserted
	// -- 0 - 4 one for each possible grid location
	int insert(const SDL_Rect &pos, const T &obj);

	// inserts an rotated object into the grid at location pos
	// returns the amount of times the object was inserted
	// -- 0 - 4 one for each possible grid location
	int insert(const SDL_Rect &pos, const double &rotation, const T &obj);

	// removes an object from the grid
	// returns the amount of times the object was removed
	// -- 0 - 4 one for each possible grid location
	int remove(const SDL_Rect &pos, const T &obj);

	// resizes the grid and clears all objects from each cell
	void resize(int cell_width, int cell_height, int row_width, int height);

	// clears all objects from each cell
	void clear();

	// returns array of pointers to each array of objects from 4 possible grid locations
	// -- top_left, top_right, botttom_left, or bottom_right
	std::vector<std::vector<T> *> get_cells(const SDL_Rect &pos);

//	void print();

	void draw(std::vector<std::pair<SDL_Rect, SDL_Color>> &rects);
private:
	bool remove_from_cell(std::vector<T> &cell, const T &obj);
private:
	int _cell_width;
	int _cell_height;
	float _inv_cell_width; // inverted  -- 1 / cell_width for multiplication instead of division
	float _inv_cell_height; // inverted    this is dumb :)

	int _width;
	int _height;
	int _max_width;
	int _max_height;

	std::vector<T> **_cells;
	std::vector<T> _large_objects;
};

template <class T>
Grid<T>::Grid() :
	_cell_width(0),
	_cell_height(0),
	_inv_cell_width(0.0f),
	_inv_cell_height(0.0f),
	_width(0),
	_height(0),
	_max_width(_width * 2),
	_max_height(_height * 2),
	_cells(nullptr)
{}

template <class T>
Grid<T>::Grid(int cell_width, int cell_height, int width, int height) :
	_cell_width(cell_width),
	_cell_height(cell_height),
	_inv_cell_width(1.0f / (float)cell_width),
	_inv_cell_height(1.0f / (float)cell_height),
	_width(width),
	_height(height),
	_max_width(width * 2),
	_max_height(height * 2)
{
	_cells = new std::vector<T> *[_height];
	for (int i = 0; i < height; ++i) {
		_cells[i] = new std::vector<T>[_width];
	}
}

template <class T>
Grid<T>::~Grid() {
	for (int i = 0; i < _height; ++i) {
		delete[] _cells[i];
	}
	delete[] _cells;
}

template <class T>
int Grid<T>::insert(const SDL_Rect &pos, const T &obj) {
	const int y = int(pos.y * _inv_cell_height);
	const int y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int x = int(pos.x * _inv_cell_width);
	const int x2 = int((pos.x + pos.w) * _inv_cell_width);
	int inserted = 0;

	if ((y2 - y) > _max_height || (x2 - x) > _max_width) {
		_large_objects.push_back(obj);
		return 1;
	}

	if (!(y > _height - 1 || y < 0)) {
		if (!(x > _width - 1 || x < 0)) {
			_cells[y][x].push_back(obj);
			++inserted;
		}

		if (x2 != x) {
			if (!(x2 > _width - 1 || x < 0)) {
				_cells[y][x2].push_back(obj);
				++inserted;
			}
		}
	}

	if (y2 != y) {
		if (!(y2 > _height - 1 || y < 0)) {
			if (!(x > _width - 1 || x < 0)) {
				_cells[y2][x].push_back(obj);
				++inserted;
			}

			if (x2 != x) {
				if (!(x2 > _width - 1 || x2 < 0)) {
					_cells[y2][x2].push_back(obj);
					++inserted;
				}
			}
		}
	}

	return inserted;
}

template <class T>
int Grid<T>::insert(const SDL_Rect &pos, const double &rotation, const T &obj) {
	std::array<Vertex, 4> vertices = make_vertices(pos, rotation);
	for (auto &vertex : vertices) {
		vertex.x = int(vertex.x * _inv_cell_width);
		vertex.y = int(vertex.y * _inv_cell_height);
	}
	int inserted = 0;

	if ((vertices[1].y - vertices[0].y) > _max_height || (vertices[2].x - vertices[0].x) > _max_width) {
		_large_objects.push_back(obj);
		return 1;
	}

	for (auto &vertex : vertices) {
		if (inserted == 0) {
			if (!(vertex.y > _height - 1 || vertex.y < 0)) {
				if (!(vertex.x > _width - 1 || vertex.x < 0)) {
					_cells[(int)vertex.y][(int)vertex.x].push_back(obj);
					++inserted;
				}
			}
		}
		else {
			bool unique = true;
			for (int i = inserted - 1; i >= 0; --i) {
				unique = !(vertex.x == vertices[i].x && vertex.y == vertices[i].y);
			}
			if (unique) {
				if (!(vertex.y > _height - 1 || vertex.y < 0)) {
					if (!(vertex.x > _width - 1 || vertex.x < 0)) {
						_cells[(int)vertex.y][(int)vertex.x].push_back(obj);
						++inserted;
						break;
					}
				}
			}
		}
	}

	if (inserted && vertices[0].y >= 0 && vertices[0].x >= 0 && vertices[0].y < _height - 1 && vertices[0].x < _width - 1) {
		if ((rotation > 0 && rotation <= 90)) {
			SDL_Rect cell_below = { int(_cell_width * vertices[0].x), int(_cell_height * vertices[0].y + 1), _cell_width, _cell_height };
			SDL_Rect cell_left = { int(_cell_width * vertices[0].x - 1), int(_cell_height * vertices[0].y), _cell_width, _cell_height };
			if (vertices[0].y + 1 < _height - 1) {
				_cells[(int)vertices[0].y + 1][(int)vertices[0].x].push_back(obj);
				++inserted;
			}
			if (vertices[0].x - 1 >= 0) {
				_cells[(int)vertices[0].y][(int)vertices[0].x - 1].push_back(obj);
				++inserted;
			}
		}
		else if ((rotation > 90 && rotation <= 180)) {
			SDL_Rect cell_above = { int(_cell_width * vertices[0].x), int(_cell_height * vertices[0].y - 1), _cell_width, _cell_height };
			SDL_Rect cell_left = { int(_cell_width * vertices[0].x - 1), int(_cell_height * vertices[0].y), _cell_width, _cell_height };
			if (vertices[0].y - 1 >= 0) {
				_cells[(int)vertices[0].y - 1][(int)vertices[0].x].push_back(obj);
				++inserted;
			}
			if (vertices[0].x - 1 >= 0) {
				_cells[(int)vertices[0].y][(int)vertices[0].x - 1].push_back(obj);
				++inserted;
			}
		}
		else if ((rotation > 180 && rotation <= 270)) {
			SDL_Rect cell_above = { int(_cell_width * vertices[0].x), int(_cell_height * vertices[0].y - 1), _cell_width, _cell_height };
			SDL_Rect cell_right = { int(_cell_width * vertices[0].x + 1), int(_cell_height * vertices[0].y), _cell_width, _cell_height };

			if (vertices[0].y - 1 >= 0) {
				_cells[(int)vertices[0].y - 1][(int)vertices[0].x].push_back(obj);
				++inserted;
			}
			if (vertices[0].x + 1 < _width - 1) {
				_cells[(int)vertices[0].y][(int)vertices[0].x + 1].push_back(obj);
				++inserted;
			}
		}
		else if ((rotation > 270 && rotation < 360)) {
			SDL_Rect cell_below = { int(_cell_width * vertices[0].x), int(_cell_height * vertices[0].y + 1), _cell_width, _cell_height };
			SDL_Rect cell_right = { int(_cell_width * vertices[0].x + 1), int(_cell_height * vertices[0].y), _cell_width, _cell_height };
			if (vertices[0].y + 1 < _height - 1) {
				_cells[(int)vertices[0].y + 1][(int)vertices[0].x].push_back(obj);
				++inserted;
			}
			if (vertices[0].x + 1 < _width - 1) {
				_cells[(int)vertices[0].y][(int)vertices[0].x + 1].push_back(obj);
				++inserted;
			}
		}
	}

	return inserted;
}

template <class T>
int Grid<T>::remove(const SDL_Rect &pos, const T &obj) {
	const int y = int(pos.y * _inv_cell_height);
	const int y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int x = int(pos.x * _inv_cell_width);
	const int x2 = int((pos.x + pos.w) * _inv_cell_width);
	int removed = 0;

	if (!(y > _height - 1 || y < 0)) {
		if (!(x > _width - 1 || x < 0)) {
			removed += remove_from_cell(_cells[y][x], obj);
		}

		if (x2 != x) {
			if (!(x2 > _width - 1 || x2 < 0)) {
				removed += remove_from_cell(_cells[y][x2], obj);
			}
		}
	}

	if (y2 != y) {
		if (!(y2 > _height - 1 || y < 0)) {
			if (!(x > _width - 1 || x < 0)) {
				removed += remove_from_cell(_cells[y2][x], obj);
			}

			if (x2 != x) {
				if (!(x2 > _width - 1 || x < 0)) {
					removed += remove_from_cell(_cells[y2][x2], obj);
				}
			}
		}
	}

	return removed;
}

template <class T>
void Grid<T>::resize(int cell_width, int cell_height, int width, int height) {
	_cell_width = cell_width;
	_cell_height = cell_height;
	_inv_cell_width = 1.0f / (float)_cell_width;
	_inv_cell_height = 1.0f / (float)_cell_height;

	if (_cells) {
		for (int i = 0; i < _height; ++i) {
			delete[] _cells[i];
		}
	}
	delete[] _cells;

	_large_objects.clear();

	_width = width;
	_max_width = width * 2;
	_height = height;
	_max_height = height * 2;

	_cells = new std::vector<T> *[_height];
	for (int i = 0; i < height; ++i) {
		_cells[i] = new std::vector<T>[_width];
	}
}

template <class T>
void Grid<T>::clear() {
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			_cells[y][x].clear();
		}
	}

	_large_objects.clear();
}

template <class T>
std::vector<std::vector<T> *> Grid<T>::get_cells(const SDL_Rect &pos) {
	std::vector<std::vector<T> *> cells;  // all possible cell locations
	std::vector<T> *cell_ptr = nullptr;  // cell for each of the 4 possible grid locations

	const int y = int(pos.y * _inv_cell_height);
	const int y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int x = int(pos.x * _inv_cell_width);
	const int x2 = int((pos.x + pos.w) * _inv_cell_width);
	int return_val = 0;

	cells.push_back(&_large_objects);

	if (!(y > _height - 1 || y < 0)) {
		if (!(x > _width - 1 || x < 0)) {
			cell_ptr = &(_cells[y][x]);
			if (cell_ptr) {
				cells.push_back(cell_ptr);
			}
		}

		if (x2 != x) {
			if (!(x2 > _width - 1 || x2 < 0)) {
				cell_ptr = &(_cells[y][x2]);
				if (cell_ptr) {
					cells.push_back(cell_ptr);
				}
			}
		}
	}

	if (y2 != y) {
		if (!(y2 > _height - 1 || y < 0)) {
			if (!(x > _width - 1 || x < 0)) {
				cell_ptr = &(_cells[y2][x]);
				if (cell_ptr) {
					cells.push_back(cell_ptr);
				}
			}

			if (x2 != x) {
				if (!(x2 > _width - 1 || x2 < 0)) {
					cell_ptr = &(_cells[y2][x2]);
					if (cell_ptr) {
						cells.push_back(cell_ptr);
					}
				}
			}
		}
	}

	return cells;
}

/*

template <class T>
void Grid<T>::print() {
	std::cout << "Row Size: " << _height << std::endl;
	for (int i = 0; i < _height; ++i) {
		std::cout << "Row: " << std::endl;
		for (int k = 0; k < _rows[i]._cell_size; ++k) {
			std::cout << "     Cell: " << std::endl;
			std::cout << "          size : " << _rows[i]._cells[k]._objs.size() << std::endl;
		}
	}
}

*/

template <class T>
void Grid<T>::draw(std::vector<std::pair<SDL_Rect, SDL_Color>> &rects) {
	for (int y = 0; y < _height; ++y) {
		for (int x = 0; x < _width; ++x) {
			SDL_Rect rect = { x * _cell_width, y * _cell_height, _cell_width, _cell_height };
			SDL_Color color = { 0, 0, 0, 150 };
			Uint8 size = (Uint8)_cells[y][x].size();
			if (size > 0) {
				color = { Uint8(2 * size), 0, Uint8(2 * size), 150 };
			}
			rects.push_back(std::pair<SDL_Rect, SDL_Color>(rect, color));
		}
	}
}

template <class T>
bool Grid<T>::remove_from_cell(std::vector<T> &cell, const T &obj) {
	for (auto it = cell.begin(); it != cell.end(); ++it) {
		if (*it == obj) {
			cell.erase(it);
			return true;
		}
	}
	return false;
}

#endif