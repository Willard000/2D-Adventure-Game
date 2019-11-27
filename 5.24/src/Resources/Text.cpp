#include "Text.h"

#include "Environment.h"
#include "Window.h"

Text::Text() :
	_text			( "" ),
	_color			( { 255, 255, 255, 255 } ),
	_font_size		( 12 ),
	_wrap_length    ( 1000 ),
	_texture		( nullptr ),
	_x				( 0 ),
	_y				( 0 ),
	_rect			( { 0, 0, 0, 0 } ),
	_is_loaded		( false )
{}

Text::Text(std::string text, SDL_Color color, int font_size, Uint32 wrap_length, int x, int y, bool ui_element) :
	_text			( text ),
	_color			( color ),
	_font_size		( font_size ),
	_wrap_length    ( wrap_length ),
	_texture		( nullptr ),
	_x				( x ),
	_y				( y ),
	_ui_element		( ui_element ),
	_rect			( { x, y, 0, 0 } ),
	_is_loaded		( false )
{}

Text &Text::operator=(const Text& rhs) {

	_text = rhs._text;
	_color = rhs._color;
	_font_size = rhs._font_size;
	_wrap_length = _wrap_length;
	_x = rhs._x;
	_y = rhs._y;
	_ui_element = rhs._ui_element;
	_rect = { _x, _y, 0, 0 };

	if (_texture)
		SDL_DestroyTexture(_texture);

	_texture = nullptr;

	_is_loaded = false;

	return *this;
}

Text::~Text() {
	SDL_DestroyTexture(_texture);
}

void Text::set_text(std::string text) {
	_text = text;
	_is_loaded = false;
	_rect.x += _rect.w / FONT_TEXT_XOFFSET_FACTOR;
	_rect.y += _rect.h / FONT_TEXT_YOFFSET_FACTOR;
}

// FLAGS
// DRAW_TEXT_NON_CENTER 1
void Text::load(int flag) {
	if (_text == "") {
		return;
	}
	if (_texture) {
		SDL_DestroyTexture(_texture);
	}
	SDL_Surface *surface = nullptr;

	if (flag == DRAW_TEXT_NON_CENTER) {
		surface = TTF_RenderText_Blended(Environment::get().get_window()->get_renderer()->get_font(), _text.c_str(), _color);
	}
	else {
		surface = TTF_RenderText_Blended_Wrapped(Environment::get().get_window()->get_renderer()->get_font(), _text.c_str(), _color, _wrap_length);
	}

	SDL_SetSurfaceAlphaMod(surface, _color.a);
	_texture = SDL_CreateTextureFromSurface(Environment::get().get_window()->get_renderer()->get_renderer(), surface);

	if (!_texture) {
		SDL_FreeSurface(surface);
		return;
	}

	_rect.w = int((float)surface->clip_rect.w * ((float)_font_size / (float)FONT_PTSIZE));
	_rect.h = int((float)surface->clip_rect.h * ((float)_font_size / (float)FONT_PTSIZE));

	if (flag != 1) {
		_rect.x -= _rect.w / FONT_TEXT_XOFFSET_FACTOR;
		_rect.y -= _rect.h / FONT_TEXT_YOFFSET_FACTOR;
	}

	SDL_FreeSurface(surface);
	_is_loaded = true;
}