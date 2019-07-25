#include <iostream>
#include <SDL.h>
#include <vector>

#ifndef GRID_H
#define GRID_H

template <class T>
class Grid {
public:
	Grid();
	Grid(int cell_width, int cell_height, int row_width, int height);
	~Grid();

	// inserts an object into the grid at location pos
	// returns the amount of times the object was removed
	// -- 0 - 4 one for each possible grid location
	int insert(const SDL_Rect &pos, const T &obj);

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
	float _inv_cell_width;
	float _inv_cell_height;

	int _width;
	int _height;

	std::vector<T> **_cells;
};

template <class T>
Grid<T>::Grid() :
	_cell_width(0),
	_cell_height(0),
	_inv_cell_width(0.0f),
	_inv_cell_height(0.0f),
	_width(0),
	_height(0),
	_cells(nullptr)
{}

template <class T>
Grid<T>::Grid(int cell_width, int cell_height, int width, int height) :
	_cell_width(cell_width),
	_cell_height(cell_height),
	_inv_cell_width(1.0f / (float)cell_width),
	_inv_cell_height(1.0f / (float)cell_height),
	_width(width),
	_height(height)
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
	const int &y = int(pos.y * _inv_cell_height);
	const int &y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int &x = int(pos.x * _inv_cell_width);
	const int &x2 = int((pos.x + pos.w) * _inv_cell_width);
	int inserted = 0;

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
int Grid<T>::remove(const SDL_Rect &pos, const T &obj) {
	const int &y = int(pos.y * _inv_cell_height);
	const int &y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int &x = int(pos.x * _inv_cell_width);
	const int &x2 = int((pos.x + pos.w) * _inv_cell_width);
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

	_width = width;
	_height = height;

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
}

template <class T>
std::vector<std::vector<T> *> Grid<T>::get_cells(const SDL_Rect &pos) {
	std::vector<std::vector<T> *> cells;  // all possible cell locations
	std::vector<T> *cell_ptr = nullptr;  // cell for each of the 4 possible grid locations

	const int &y = int(pos.y * _inv_cell_height);
	const int &y2 = int((pos.y + pos.h) * _inv_cell_height);
	const int &x = int(pos.x * _inv_cell_width);
	const int &x2 = int((pos.x + pos.w) * _inv_cell_width);
	int return_val = 0;

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
				color = { Uint8(5 * size), 100, Uint8(5 * size), 150 };
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