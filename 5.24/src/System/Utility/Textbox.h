#include <string>
#include <list>

#include <SDL_rect.h>

#include "Text.h"
#include "Renderer.h"

#include "Timer.h"

#ifndef TEXTBOX_H
#define TEXTBOX_H

#define TEXTBOX_TEXT_COLOR { 255, 255, 255, 200 }
#define TEXTBOX_FT_SIZE 18

class TextBox {

public:
	TextBox();
	TextBox(SDL_Rect rect, SDL_Color color, int ft_size = TEXTBOX_FT_SIZE);
	~TextBox();

	void print(std::string line, SDL_Color color = TEXTBOX_TEXT_COLOR);
	void clear();

	void draw(Renderer *renderer);

	void update();

	void toggle();

	const SDL_Rect &get_rect() { return _rect; }
	const SDL_Color &get_color() { return _color; }
	bool get_show() { return _show; }
private:
	std::list<Text *> _lines;
	SDL_Rect _rect;
	SDL_Color _color;

	int _ft_size;
	int _wrap_length;
	int _y_spacing;

	bool _show;

	Timer _show_timer;
};

#endif