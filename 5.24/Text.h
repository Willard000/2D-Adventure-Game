#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#ifndef TEXT_H
#define TEXT_H

struct Text {
	Text();
	Text(std::string text_, SDL_Color color_, int font_size_, Uint32 wrap_length_, int x_, int y_);
	~Text();

	Text &operator=(const Text &rhs);

	std::string text;
	SDL_Color color;
	int font_size;
	Uint32 wrap_length;
	SDL_Texture *texture;
	SDL_Rect rect;
};

#endif