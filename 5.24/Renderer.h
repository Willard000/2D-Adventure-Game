#include <string>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "Map.h"
#include "Camera.h"
#include "Text.h"

#ifndef RENDERER_H
#define RENDERER_H

#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#define RGB_DEPTH 32

class Renderer {
public:
	Renderer(SDL_Window *window, SDL_Color backgroundColor, Camera *camera);
	~Renderer();

	void render();
	void clear();

	SDL_Texture *createTexture(std::string path);
	SDL_Surface *createSurface(std::string path);

	void render(const Texture *img);
	void render(const Texture *img, const SDL_Rect &pos, bool ui_element = false);
	void render(const Texture *img, PositionComponent *position);
	void render(Sprite *img, SpriteComponent *sprite, PositionComponent *position);

	void drawText(Text &text, bool ui_element = false);
	void drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag = DRAW_RECT_FULL);
	void drawLine(const SDL_Rect &rect, const SDL_Color &color);

	SDL_Texture *makeBlitTexture(std::map<int, SDL_Surface *> &surfaces, const int &width, const int &height, const int &surface_size);
	SDL_Texture *blitTexture(SDL_Surface *&main_surface, SDL_Surface *surface, SDL_Rect &pos);

	void createText(Text &text);

	SDL_Renderer *getRenderer() { return _renderer; }
public:
	enum {
		DRAW_RECT_EMPTY,
		DRAW_RECT_FULL
	};

private:
	// Offsets rect with camera coordinates
	SDL_Rect getDestination(const SDL_Rect &position);
private:
	const SDL_Color NULL_TEXTURE_COLOR = { 200, 200, 255, 255 };
	const SDL_Color NULL_TEXT_COLOR = { 255, 200, 200, 255 };

	SDL_Renderer *_renderer;
	SDL_Color _backgroundColor;
	TTF_Font *_font;

	Camera *_camera;
};

#endif