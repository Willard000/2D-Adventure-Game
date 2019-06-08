#include <memory>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "SpriteComponent.h"

#ifndef RENDERER_H
#define RENDERER_H

#define COLOR_KEY_R 0x00
#define COLOR_KEY_G 0x00
#define COLOR_KEY_B 0x0ff

class Renderer {
public:
	Renderer(SDL_Window *window, SDL_Rect background, SDL_Color backgroundColor);
	~Renderer();

	void render() { SDL_RenderPresent(_renderer); }
	void clear() { SDL_RenderClear(_renderer); }

	SDL_Texture *createTexture(std::string path);
	void render(const Texture *img, const SDL_Rect &pos);
	void render(Texture *img, const SDL_Rect &pos, SpriteComponent *sprite);

	void drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag = DRAW_RECT_FULL);
	void drawBackground() { drawRect(_background, _backgroundColor); }

public:
	enum {
		DRAW_RECT_EMPTY,
		DRAW_RECT_FULL
	};
private:
	const SDL_Color PLACE_HOLDER_COLOR = { 255, 204, 255, 255 };

	SDL_Renderer *_renderer;
	SDL_Rect _background;
	SDL_Color _backgroundColor;
};

#endif