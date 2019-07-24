// testing
//#include <iostream>
//#include <SDL.h>
//#include <vector>
// testing

#include "Entity.h"
#include "PositionComponent.h"

#ifndef GRID_H
#define GRID_H

template <class T>
class Grid {
public:
	class Cell {
	public:
		Cell();
		~Cell();
		Cell &operator=(const Cell &rhs);

		bool insert(const T &obj);

		bool remove(const T &obj);

		std::vector<T> *get_objs(const SDL_Rect &pos);

		friend class Row;
		friend Grid; // test for print/draw
	private:
		std::vector<T> _objs;
	};

	class Row {
	public:
		Row();
		Row(int width);
		~Row();
		Row &operator=(const Row &rhs);

		bool insert(const int &index, const T &obj);

		bool remove(const int &index, const T &obj);

		void clear();

		std::vector<T> *get_objs(const int &index, const SDL_Rect &pos);

		friend Grid; // test for print/draw
	private:
		Cell *_cells;
		int _cell_size;
	};
public:
	Grid();
	Grid(int cell_width, int cell_height, int row_width, int height);
	~Grid();

	bool insert(const int &x, const int &y, const T &obj);

	bool remove(const int &x, const int &y, const T &obj);

	void resize(int cell_width, int cell_height, int row_width, int height);

	void clear();

	std::vector<T> *get_objs(const SDL_Rect &pos);

	void print();

	void draw(std::vector<std::pair<SDL_Rect, SDL_Color>> &rects);
private:
	Row *_rows;
	int _row_size;

	int _cell_width;
	int _cell_height;

	int _width;
	int _height;
};

//****************************************CELL*******************************************************//
template <class T>
Grid<T>::Cell::Cell()
{}

template <class T>
Grid<T>::Cell::~Cell()
{}

template <class T>
typename Grid<T>::Cell &Grid<T>::Cell::operator=(const Grid<T>::Cell &rhs) {
	_objs = rhs._objs;

	return *this;
}

template <class T>
bool Grid<T>::Cell::insert(const T &obj) {
	_objs.push_back(obj);
	return true;
}

template <class T>
bool Grid<T>::Cell::remove(const T &obj) {
	for (auto it = _objs.begin(); it != _objs.end(); ++it) {
		if (*it == obj) {
			_objs.erase(it);
			return true;
		}
	}
	return false;
}

template <class T>
std::vector<T> *Grid<T>::Cell::get_objs(const SDL_Rect &pos) {
	return &_objs;
}

//****************************************ROW********************************************************//
template <class T>
Grid<T>::Row::Row() :
	_cells			( nullptr ),
	_cell_size		( 0 )
{}

template <class T>
Grid<T>::Row::Row(int width) :
	_cells			( new Cell[width] ),
	_cell_size		( width )
{
	for (int i = 0; i < width; ++i) {
		_cells[i] = Cell();
	}
}

template <class T>
Grid<T>::Row::~Row() {
	if (_cells)
		delete[] _cells;

	_cells = nullptr;
	_cell_size = 0;
}

template <class T>
typename Grid<T>::Row &Grid<T>::Row::operator=(const Grid<T>::Row &rhs) {
	if (_cells) {
		delete[] _cells;
		_cells = nullptr;
	}

	_cell_size = rhs._cell_size;
	_cells = new Cell[_cell_size];
	for (int i = 0; i < _cell_size; ++i) {
		_cells[i] = rhs._cells[i];
	}

	return *this;
}

template <class T>
bool Grid<T>::Row::insert(const int &index, const T &obj) {
	if (index < 0 || index > _cell_size - 1) {
		return false;
	}

	return _cells[index].insert(obj);
}

template <class T>
bool Grid<T>::Row::remove(const int &index, const T &obj) {
	if (index < 0 || index > _cell_size - 1) {
		return false;
	}

	return _cells[index].remove(obj);
}

template <class T>
void Grid<T>::Row::clear() {
	for (int i = 0; i < _cell_size; ++i) {
		_cells[i]._objs.clear();
	}
}

template <class T>
std::vector<T> *Grid<T>::Row::get_objs(const int &index, const SDL_Rect &pos) {
	if (index < 0 || index > _cell_size - 1) {
		return nullptr;
	}

	return _cells[index].get_objs(pos);
}

//****************************************GRID*******************************************************//
template <class T>
Grid<T>::Grid() :
	_rows			( nullptr ),
	_row_size		( 0 ),
	_cell_width		( 0 ),
	_cell_height	( 0 ),
	_width			( 0 ),
	_height			( 0 )
{}

template <class T>
Grid<T>::Grid(int cell_width, int cell_height, int width, int height) :
	_rows			( new Row[height] ),
	_row_size		( height ),
	_cell_width		( cell_width ),
	_cell_height	( cell_height ),
	_width			( width ),
	_height			( height )
{
	for (int i = 0; i < height; ++i) {
		_rows[i] = Row(width);
	}
}

template <class T>
Grid<T>::~Grid() {
	if (_rows)
		delete[] _rows;

	_rows = nullptr;
	_row_size = 0;
}

template <class T>
bool Grid<T>::insert(const int &x, const int &y, const T &obj) {
	const int x_index = x / _cell_width;
	const int y_index = y / _cell_height;
	if (y_index < 0 || y_index > _row_size - 1) {
		return false;
	}

	return _rows[y_index].insert(x_index, obj);
}

template <class T>
bool Grid<T>::remove(const int &x, const int &y, const T &obj) {
	const int x_index = x / _cell_width;
	const int y_index = y / _cell_height;
	if (y_index < 0 || y_index > _row_size - 1) {
		return false;
	}

	return _rows[y_index].remove(x_index, obj);
}

template <class T>
void Grid<T>::resize(int cell_width, int cell_height, int width, int height) {
	if (_rows)
		delete[] _rows;

	_rows = new Row[height];
	_row_size = height;

	_cell_width = cell_width;
	_cell_height = cell_height;

	_width = width;
	_height = height;

	for (int i = 0; i < height; ++i) {
		_rows[i] = Row(width);
	}
}

template <class T>
void Grid<T>::clear() {
	for (int i = 0; i < _row_size; ++i) {
		_rows[i].clear();
	}
}

template <class T>
std::vector<T> *Grid<T>::get_objs(const SDL_Rect &pos) {
	const int x_index = pos.x / _cell_width;
	const int y_index = pos.y / _cell_height;
	if (y_index < 0 || y_index > _row_size - 1) {
		return nullptr;
	}

	return _rows[y_index].get_objs(x_index, pos);
}

template <class T>
void Grid<T>::print() {
	std::cout << "Row Size: " << _row_size << std::endl;
	for (int i = 0; i < _row_size; ++i) {
		std::cout << "Row: " << std::endl;
		for (int k = 0; k < _rows[i]._cell_size; ++k) {
			std::cout << "     Cell: " << std::endl;
			std::cout << "          size : " << _rows[i]._cells[k]._objs.size() << std::endl;
		}
	}
}

template <class T>
void Grid<T>::draw(std::vector<std::pair<SDL_Rect, SDL_Color>> &rects) {
	for (int i = 0; i < _row_size; ++i) {
		for (int k = 0; k < _rows[i]._cell_size; ++k) {
			SDL_Rect rect = { k * _cell_width, i * _cell_height, _cell_width, _cell_height};
			SDL_Color color = { 0, 0, 0, 150 };
			if (_rows[i]._cells[k]._objs.size() > 0)
				color = { 255, 255, 255, 150 };
			rects.push_back(std::pair<SDL_Rect, SDL_Color>(rect, color));
		}
	}
}

#endif