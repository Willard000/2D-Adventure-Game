#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#ifndef TEXT_H
#define TEXT_H

class Text {
public:
	Text();
	Text(std::string text, SDL_Color color, int font_size, Uint32 wrap_length, int x, int y);
	~Text();

	Text &operator=(const Text &rhs);

	const std::string &get_text() { return _text; }

	void set_text(std::string text);

	void load();

	friend class Renderer;
private:
	std::string _text;
	SDL_Color _color;
	int _font_size;
	Uint32 _wrap_length;
	SDL_Texture *_texture;
	int _x, _y;
	SDL_Rect _rect;
	bool _is_loaded;
};

#endif