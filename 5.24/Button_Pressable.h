#include "Button.h"

#include <string>

#ifndef BUTTON_PRESSABLE_H
#define BUTTON_PRESSABLE_H

namespace UI {

	template <class T>
	class Button_Pressable : public Button {
	public:
		typedef void (T::*MemberFunction)(void);

		Button_Pressable(T *instance, MemberFunction onPress, std::string text, SDL_Color text_color, int font_size, Uint32 wrap_length, SDL_Rect rect, SDL_Color color) :
			Button			( text, text_color, font_size, wrap_length, rect, color ),
			_instance		( instance ),
			_onPress		( onPress )
		{}

		void execute() { (_instance->*_onPress)(); }
	private:
		T *_instance;
		MemberFunction _onPress;
	};

}

#endif