#include "Button.h"

#ifndef BUTTON_PRESSABLE_H
#define BUTTON_PRESSABLE_H

namespace UI {

	class Button_Pressable : public Button {
	public:
		typedef void (*CallBack)(void);

		Button_Pressable(
			CallBack on_press,
			std::string text = "Text",
			SDL_Rect rect = { 0, 0, 100, 20 },
			int font_size = 16,
			Uint32 wrap_length = 1000,
			SDL_Color text_color = { 255, 255, 255, 255 },
			SDL_Color color = {30, 30, 30, 80}
		) :
			Button			( text, text_color, font_size, wrap_length, rect, color ),
			_on_press		( on_press )
		{}

		void execute() { _on_press(); }
	private:
		CallBack _on_press;
	};

}

#endif