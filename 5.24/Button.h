
#include <SDL.h>

#ifndef BUTTON_H
#define BUTTON_H

namespace UI {

	class Button {
	public:
		Button(SDL_Rect rect, SDL_Color color) :
			_rect		( rect ),
			_color		( color )
		{}

		virtual void execute() {}

		virtual const SDL_Rect getRect() { return _rect; }
		virtual const SDL_Color getColor() { return _color; }
	private:
		SDL_Rect _rect;
		SDL_Color _color;
	};

	template <class T>
	class ButtonPress : public Button {
	public:
		typedef void (T::*MemberFunction)(void);

		ButtonPress(T *instance, MemberFunction onPress, SDL_Rect rect, SDL_Color color) :
			Button(rect, color),
			_instance(instance),
			_onPress(onPress)
		{}

		void execute() { (_instance->*_onPress)(); }

	private:
		T *_instance;
		MemberFunction _onPress;
	};

}

#endif