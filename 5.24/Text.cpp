#include "Text.h"

#include "Environment.h"
#include "WindowManager.h"

Text::Text() :
	text			( "" ),
	color			( { 255, 255, 255, 255 } ),
	font_size		( 12 ),
	wrap_length     ( 1000 ),
	texture			( nullptr ),
	rect			( { 0, 0, 0, 0 } )
{}

Text::Text(std::string text_, SDL_Color color_, int font_size_, Uint32 wrap_length_, int x_, int y_) :
	text			( text_ ),
	color			( color_ ),
	font_size		( font_size_ ),
	wrap_length     ( wrap_length_ ),
	texture			( nullptr ),
	rect			( { x_, y_, 0, 0 } )
{
	Environment::get().getWindowManager()->getRenderer()->createText(*this);
}

Text &Text::operator=(const Text& rhs) {

	text = rhs.text;
	color = rhs.color;
	font_size = rhs.font_size;
	wrap_length = wrap_length;
	rect = rhs.rect;

	if (texture)
		SDL_DestroyTexture(texture);

	Environment::get().getWindowManager()->getRenderer()->createText(*this);

	return *this;
}

Text::~Text() {
	SDL_DestroyTexture(texture);
}