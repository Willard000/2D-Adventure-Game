#include "Renderer.h"

Renderer::Renderer(SDL_Window *window, Camera *camera, SDL_Rect background, SDL_Color backgroundColor) :
	_renderer				( SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) ),
	_camera					( camera ),
	_background				( background ),
	_backgroundColor		( backgroundColor ),
	_uniform_rotation		( 0.0 ),
	_uniform_scale			( 0.0 )
{}

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
	SDL_Rect des = { 
		pos.x - (int)_camera->_x,
		pos.y - (int)_camera->_y,
		int(pos.w * (_uniform_scale + 1.0)),
		int(pos.h * (_uniform_scale + 1.0))
	};
	if (img != nullptr) {
		SDL_RenderCopyEx(_renderer, img->texture, NULL, &des, _uniform_rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		drawRect(des, PLACE_HOLDER_COLOR);
	}
}

void Renderer::render(const Texture *img, PositionComponent *position) {
	SDL_Rect des = { 
		position->m_rect.x - (int)_camera->_x,
		position->m_rect.y - (int)_camera->_y,
		int(position->m_rect.w * (_uniform_scale + position->m_scale)),
		int(position->m_rect.h * (_uniform_scale + position->m_scale))
	};
	if (img != nullptr) {
		SDL_RenderCopyEx(_renderer, img->texture, NULL, &des, _uniform_rotation + position->m_rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		drawRect(des, PLACE_HOLDER_COLOR);
	}
}

void Renderer::render(Sprite *img, SpriteComponent *sprite, PositionComponent *position) {
	SDL_Rect des = { 
		position->m_rect.x - (int)_camera->_x,
		position->m_rect.y - (int)_camera->_y,
		int(position->m_rect.w * (_uniform_scale + position->m_scale)),
		int(position->m_rect.h * (_uniform_scale + position->m_scale))
	};
	if (img != nullptr) {
		sprite->update(img);
		SDL_RenderCopyEx(_renderer, img->texture, &sprite->m_pos, &des, _uniform_rotation + position->m_rotation, NULL, SDL_FLIP_NONE);
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

void Renderer::rotate(double angle) {
	_uniform_rotation += angle;
}

void Renderer::setRotation(double angle) {
	_uniform_rotation = angle;
}

void Renderer::scale(double factor) {
	_uniform_scale += factor;
}

void Renderer::setScale(double factor) {
	_uniform_scale = factor;
}