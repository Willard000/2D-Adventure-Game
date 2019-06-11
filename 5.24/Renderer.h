#include <memory>
#include <string>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "SpriteComponent.h"
#include "Map.h"
#include "Camera.h"

#ifndef RENDERER_H
#define RENDERER_H

#define COLOR_KEY_R 0x00
#define COLOR_KEY_G 0x00
#define COLOR_KEY_B 0x0ff

#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#define RGB_DEPTH 32

class Renderer {
public:
	Renderer(SDL_Window *window, Camera *camera, SDL_Rect background, SDL_Color backgroundColor);
	~Renderer();

	void render() { SDL_RenderPresent(_renderer); }
	void clear() { SDL_RenderClear(_renderer); }

	SDL_Texture *createTexture(std::string path);
	SDL_Surface *createSurface(std::string path);
	void render(const Texture *img, const SDL_Rect &pos);
	void render(Sprite *img, const SDL_Rect &pos, SpriteComponent *sprite);

	void drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag = DRAW_RECT_FULL);
	void drawBackground() { drawRect(_background, _backgroundColor); }

	SDL_Texture *createMapTexture(std::vector<Tile> &tiles, std::map<int, SDL_Surface *> &tile_surfaces, const int &width, const int &height);

public:
	enum {
		DRAW_RECT_EMPTY,
		DRAW_RECT_FULL
	};
private:
	const SDL_Color PLACE_HOLDER_COLOR = { 255, 204, 255, 255 };

	SDL_Renderer *_renderer;
	Camera *_camera;
	SDL_Rect _background;
	SDL_Color _backgroundColor;
};

#endif