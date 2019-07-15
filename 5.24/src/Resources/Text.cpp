#include "Text.h"

#include "Environment.h"
#include "Window.h"

Text::Text() :
	text			( "" ),
	color			( { 255, 255, 255, 255 } ),
	font_size		( 12 ),
	wrap_length     ( 1000 ),
	texture			( nullptr ),
	x				( 0 ),
	y				( 0 ),
	rect			( { 0, 0, 0, 0 } ),
	loaded			( false )
{}

Text::Text(std::string text_, SDL_Color color_, int font_size_, Uint32 wrap_length_, int x_, int y_) :
	text			( text_ ),
	color			( color_ ),
	font_size		( font_size_ ),
	wrap_length     ( wrap_length_ ),
	texture			( nullptr ),
	x				( x_ ),
	y				( y_ ),
	rect			( { x_, y_, 0, 0 } ),
	loaded			( false )
{}

Text &Text::operator=(const Text& rhs) {

	text = rhs.text;
	color = rhs.color;
	font_size = rhs.font_size;
	wrap_length = wrap_length;
	x = rhs.x;
	y = rhs.y;
	rect = { x, y, 0, 0 };

	if (texture)
		SDL_DestroyTexture(texture);

	loaded = false;

	return *this;
}

Text::~Text() {
	SDL_DestroyTexture(texture);
}