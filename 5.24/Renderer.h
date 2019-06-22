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

class Renderer {
public:
	Renderer(SDL_Window *window, SDL_Color backgroundColor, Camera *camera);
	~Renderer();

	void render();
	void clear();

	SDL_Texture *createTexture(std::string path);
	SDL_Surface *createSurface(std::string path);

	void render(const Texture *img, const SDL_Rect &pos);
	void render(const Texture *img, PositionComponent *position);
	void render(Sprite *img, SpriteComponent *sprite, PositionComponent *position);

	void drawText(const Text &text, bool ui_element = false);
	void drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag = DRAW_RECT_FULL);

	SDL_Texture *createMapTexture(std::vector<Map::Tile> &tiles, std::map<int, SDL_Surface *> &tile_surfaces, const int &width, const int &height);
	void createText(Text &text);

	void rotate(double angle);
	void setRotation(double angle);
	void scale(float factor);
	void setScale(float factor);
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

	double _uniform_rotation;
	float _uniform_scale;
};

#endif