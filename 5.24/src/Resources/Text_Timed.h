#include "Text.h"

#include "Timer.h"

#ifndef TEXT_TIMED_H
#define TEXT_TIMED_H

class Text_Timed : public Text {
public:
	Text_Timed(
		std::string text,
		SDL_Color color,
		int font_size,
		Uint32 wrap_length,
		int x, int y,
		int time,
		bool ui_element = false
	);

	void update();

	bool end() { return _end; }

	int get_type() { return TEXT_TYPE_TIMED; }
private:
	bool _end;
	Timer _timer;
};

#endif