#include <string>
#include <map>

#include <SDL.h>
#include <SDL_image.h>

#include "Texture.h"
#include "SpriteComponent.h"
#include "PositionComponent.h"
#include "Map.h"
#include "Camera.h"

#ifndef RENDERER_H
#define RENDERER_H

class Renderer {
public:
	Renderer(SDL_Window *window, Camera *camera, SDL_Color backgroundColor);
	~Renderer();

	void render();
	void clear();

	SDL_Texture *createTexture(std::string path);
	SDL_Surface *createSurface(std::string path);
	void render(const Texture *img, const SDL_Rect &pos);
	void render(const Texture *img, PositionComponent *position);
	void render(Sprite *img, SpriteComponent *sprite, PositionComponent *position);

	void drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag = DRAW_RECT_FULL);

	SDL_Texture *createMapTexture(std::vector<Map::Tile> &tiles, std::map<int, SDL_Surface *> &tile_surfaces, const int &width, const int &height);

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
	const SDL_Color NULL_TEXTURE_COLOR = { 255, 204, 255, 255 };

	SDL_Renderer *_renderer;
	Camera *_camera;
	SDL_Color _backgroundColor;

	double _uniform_rotation;
	float _uniform_scale;
};

#endif