#include "Renderer.h"

Renderer::Renderer(SDL_Window *window, SDL_Rect background, SDL_Color backgroundColor) {
	_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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

	printf("Could not load texture at: \"%s\"", path.c_str());

	return texture;
}

void Renderer::render(Texture *img, const SDL_Rect &pos) {
	if (img != nullptr) {
		SDL_RenderCopy(_renderer, img->texture, NULL, &pos);
	}
	else {
		drawRect(pos, PLACE_HOLDER_COLOR);
	}
}

void Renderer::render(Texture *img, const SDL_Rect &pos, SpriteComponent *sprite) {
	if (img != nullptr) {
		sprite->update(img);
		SDL_RenderCopy(_renderer, img->texture, &sprite->pos, &pos);
	}
	else {
		drawRect(pos, PLACE_HOLDER_COLOR);
	}
}

void Renderer::drawRect(const SDL_Rect &rect, const SDL_Color &color, int flag) {
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderDrawRect(_renderer, &rect);
	if (flag != DRAW_RECT_EMPTY) {
		SDL_RenderFillRect(_renderer, &rect);
	}
}