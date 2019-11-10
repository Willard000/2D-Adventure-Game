#include "Textbox.h"

#define DISPLAY_TIME 5000

TextBox::TextBox() :
	_rect			{ 0, 0, 1000, 300 },
	_color			{ 50, 50, 50, 50 },
	_show			{ 0 },
	_ft_size		{ TEXTBOX_FT_SIZE },
	_wrap_length	{ _rect.w * 5 },
	_y_spacing		{ _ft_size + 2 },
	_show_timer		{ DISPLAY_TIME }
{}

TextBox::TextBox(SDL_Rect rect, SDL_Color color, int ft_size) :
	_rect			{ rect },
	_color			{ color },
	_show			{ 0 },
	_ft_size		{ ft_size },
	_wrap_length	{ _rect.w * 5 },
	_y_spacing		{ _ft_size + 2 },
	_show_timer		{ DISPLAY_TIME }
{}

TextBox::~TextBox() {
	for (auto &t : _lines) {
		delete t;
	}
}

void TextBox::update() {
	if (_show && _show_timer.update()) {
		_show = false;
	}
}

void TextBox::print(std::string line, SDL_Color color) {
	if (_lines.size() + 1 > (_rect.h / _ft_size - 1)) {
		delete _lines.front();
		_lines.pop_front();
		for (auto &t : _lines) {
			t->get_rect().y -= _y_spacing;
		}
	}

	int y = (int)_lines.size() * _y_spacing;
	Text *new_line = new Text(line, color, _ft_size, _wrap_length, _rect.x, _rect.y + y);
	_lines.push_back(new_line);

	_show = true;
	_show_timer.reset();
}

void TextBox::draw(Renderer *renderer) {
	for (auto &t : _lines) {
		renderer->draw_text(t, true, DRAW_TEXT_NON_CENTER);
	}
}

void TextBox::toggle() {
	_show = !_show;
	_show_timer.reset();
}

void TextBox::clear() {
	for (auto &t : _lines) {
		delete t;
	}
	_lines.clear();
}