#include "Renderer.h"

#define COLOR_KEY_R 0x00
#define COLOR_KEY_G 0x00
#define COLOR_KEY_B 0x0ff

#define RMASK 0xff000000
#define GMASK 0x00ff0000
#define BMASK 0x0000ff00
#define AMASK 0x000000ff
#define RGB_DEPTH 32

#define LOGICAL_SIZE_W 1920
#define LOGICAL_SIZE_H 1080

Renderer::Renderer(SDL_Window *window, Camera *camera, SDL_Color backgroundColor) :
	_renderer				( SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) ),
	_camera					( camera ),
	_backgroundColor		( backgroundColor ),
	_uniform_rotation		( 0.0 ),
	_uniform_scale			( 1.0 )
{
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(_renderer, LOGICAL_SIZE_W, LOGICAL_SIZE_H);
	SDL_RenderSetScale(_renderer, _uniform_scale, _uniform_scale);
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(_renderer);
}

void Renderer::render() {
	SDL_RenderPresent(_renderer);
}

void Renderer::clear() {
	SDL_SetRenderDrawColor(_renderer, _backgroundColor.r, _backgroundColor.g, _backgroundColor.b, _backgroundColor.a);
	SDL_RenderClear(_renderer);
}

SDL_Texture *Renderer::createTexture(std::string path) {
	SDL_Texture *texture = nullptr;
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface != nullptr) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		texture = SDL_CreateTextureFromSurface(_renderer, surface);
		SDL_FreeSurface(surface);
		return texture;
	}

	printf("Could not load texture at: \"%s\"\n", path.c_str());

	return texture;
}

SDL_Surface *Renderer::createSurface(std::string path) {
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface != nullptr) {
		return surface;
	}

	printf("Could not load surface at: \"%s\"\n", path.c_str());

	return surface;
}

void Renderer::render(const Texture *img, const SDL_Rect &pos) {
	SDL_Rect des = { 
		pos.x - (int)_camera->_x,
		pos.y - (int)_camera->_y,
		pos.w,
		pos.h
	};
	if (img != nullptr) {
		SDL_RenderCopyEx(_renderer, img->texture, NULL, &des, _uniform_rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		drawRect(des, NULL_TEXTURE_COLOR);
	}
}

void Renderer::render(const Texture *img, PositionComponent *position) {
	SDL_Rect des = { 
		position->rect.x - (int)_camera->_x,
		position->rect.y - (int)_camera->_y,
		position->rect.w,
		position->rect.h
	};
	if (img != nullptr) {
		SDL_RenderCopyEx(_renderer, img->texture, NULL, &des, _uniform_rotation + position->rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		drawRect(des, NULL_TEXTURE_COLOR);
	}
}

void Renderer::render(Sprite *img, SpriteComponent *sprite, PositionComponent *position) {
	SDL_Rect des = { 
		position->rect.x - (int)_camera->_x,
		position->rect.y - (int)_camera->_y,
		position->rect.w,
		position->rect.h
	};

	if (img != nullptr) {
		sprite->update(img);
		SDL_RenderCopyEx(_renderer, img->texture, &sprite->pos, &des, _uniform_rotation + position->rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		drawRect(des, NULL_TEXTURE_COLOR);
	}
}

void Renderer::drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag) {
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderSetScale(_renderer, 1.0f, 1.0f);						// dont apply scale to ui 
	SDL_RenderDrawRect(_renderer, &rect);
	if (flag != DRAW_RECT_EMPTY) {
		SDL_RenderFillRect(_renderer, &rect);
	}
	SDL_RenderSetScale(_renderer, _uniform_scale, _uniform_scale); // change scale back 
}

SDL_Texture *Renderer::createMapTexture(std::vector<Map::Tile> &tiles, std::map<int, SDL_Surface *> &tile_surfaces, const int &width, const int &height) {
	SDL_Surface *surface = SDL_CreateRGBSurface(NULL, width, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_Texture *texture = nullptr;
	for (unsigned int i = 0; i < tiles.size(); i++) {
		SDL_Rect pos = { 0, 0, 32, 32 };
		SDL_BlitSurface(tile_surfaces[tiles[i].id], NULL, surface, &tiles[i].pos);
	}

	texture = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void Renderer::rotate(double angle) {
	_uniform_rotation += angle;
}

void Renderer::setRotation(double angle) {
	_uniform_rotation = angle;
}

void Renderer::scale(float factor) {
	_uniform_scale += factor;
	SDL_RenderSetScale(_renderer, _uniform_scale, _uniform_scale);
}

void Renderer::setScale(float factor) {
	_uniform_scale = factor;
	SDL_RenderSetScale(_renderer, _uniform_scale, _uniform_scale);
}