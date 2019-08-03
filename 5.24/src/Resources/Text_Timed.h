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
		int time 
	);

	void update();

	bool end() { return _end; }
private:
	bool _end;
	Timer _timer;
};

#endif