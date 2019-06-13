#include "Renderer.h"

Renderer::Renderer(SDL_Window *window, Camera *camera, SDL_Rect background, SDL_Color backgroundColor) {
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	_camera = camera;
	_background = background;
	_backgroundColor = backgroundColor;
}

Renderer::~Renderer() {
	SDL_DestroyRenderer(_renderer);
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
	SDL_Rect des = { pos.x - (int)_camera->_x, pos.y - (int)_camera->_y, pos.w, pos.h };
	if (img != nullptr) {
		SDL_RenderCopy(_renderer, img->texture, NULL, &des);
	}
	else {
		drawRect(des, PLACE_HOLDER_COLOR);
	}
}

void Renderer::render(Sprite *img, const SDL_Rect &pos, SpriteComponent *sprite) {
	SDL_Rect des = { pos.x - (int)_camera->_x, pos.y - (int)_camera->_y, pos.w, pos.h };
	if (img != nullptr) {
		sprite->update(img);
		SDL_RenderCopy(_renderer, img->texture, &sprite->pos, &des);
	}
	else {
		drawRect(des, PLACE_HOLDER_COLOR);
	}
}

void Renderer::drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag) {
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(_renderer, &rect);
	if (flag != DRAW_RECT_EMPTY) {
		SDL_RenderFillRect(_renderer, &rect);
	}
}

SDL_Texture *Renderer::createMapTexture(std::vector<Tile> &tiles, std::map<int, SDL_Surface *> &tile_surfaces, const int &width, const int &height) {
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