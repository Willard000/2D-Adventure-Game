
#include <SDL.h>

#include <string>

#include "Text.h"

#ifndef UI_BUTTON_H
#define UI_BUTTON_H

namespace UI {

	class Button {
	public:
		Button(std::string text, SDL_Color text_color, int font_size, Uint32 wrap_length, SDL_Rect rect, SDL_Color color) :
			_text		( Text(text, text_color, font_size, wrap_length, rect.x + (rect.w / 2), rect.y + (rect.h / 4)) ),
			_rect		( rect ),
			_color		( color )
		{}

		virtual void execute() {}

	    const Text &getText() { return _text; }
		const SDL_Rect &getRect() { return _rect; }
		const SDL_Color &getColor() { return _color; }
	private:
		Text _text;
		SDL_Rect _rect;
 		SDL_Color _color;
	};

}

#endif