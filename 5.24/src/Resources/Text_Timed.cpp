#include "Text_Timed.h"

Text_Timed::Text_Timed(
	std::string text,
	SDL_Color color,
	int font_size,
	Uint32 wrap_length,
	int x, int y,
	int time
) :
	Text		( text, color, font_size, wrap_length, x, y ),
	_timer		( time ),
	_end		( false )
{}

void Text_Timed::update() {
	if (!_end && _timer.update()) {
		_end = true;
	}
}