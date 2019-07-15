#include "Renderer.h"

#include <cassert>

#include "Environment.h"
#include "Log.h"

#define COLOR_KEY_R 0x00
#define COLOR_KEY_G 0x00
#define COLOR_KEY_B 0x0ff

#define LOGICAL_SIZE_W 1920
#define LOGICAL_SIZE_H 1080

#define FONT_BASE_PATH "Data/Font/font.ttf"
#define FONT_PT_SIZE 92

// centers the text position depending on its length
// ex. pos.x -= pos.w / TEXT_OFFSET_FECTOR_X
#define TEXT_OFFSET_FACTOR_X 2
#define TEXT_OFFSET_FACTOR_Y 8

Renderer::Renderer(SDL_Window *window, SDL_Color background_color, Camera *camera) :
	_renderer				( SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED) ),
	_background_color		( background_color ),
	_font					( TTF_OpenFont(FONT_BASE_PATH, FONT_PT_SIZE) ),
	_camera					( camera )
{
	assert(_font);
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderSetLogicalSize(_renderer, LOGICAL_SIZE_W, LOGICAL_SIZE_H);
	SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
}

Renderer::~Renderer() {
	TTF_CloseFont(_font);
	SDL_DestroyRenderer(_renderer);
}

void Renderer::render() {
	SDL_RenderPresent(_renderer);
}

void Renderer::clear() {
	SDL_SetRenderDrawColor(_renderer, _background_color.r, _background_color.g, _background_color.b, _background_color.a);
	SDL_RenderClear(_renderer);
}

SDL_Texture *Renderer::create_texture(std::string path) {
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

SDL_Surface *Renderer::create_surface(std::string path) {
	SDL_Surface *surface = IMG_Load(path.c_str());
	if (surface != nullptr) {
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		return surface;
	}

	printf("Could not load surface at: \"%s\"\n", path.c_str());

	return surface;
}

void Renderer::render(const Texture *img, bool ui_element) {
	if (img != nullptr) {
		if (ui_element) {
			SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
			SDL_RenderCopyEx(_renderer, img->texture, NULL, &img->rect, _camera->_uniform_rotation, NULL, SDL_FLIP_NONE);
			SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);

		}
		else {
			SDL_RenderCopyEx(_renderer, img->texture, NULL, &get_des(img->rect), _camera->_uniform_rotation, NULL, SDL_FLIP_NONE);
		}
	}
}

void Renderer::render(const Texture *img, const SDL_Rect &pos, bool ui_element) {
	if (img != nullptr) {
		if (ui_element) {
			SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
			SDL_RenderCopyEx(_renderer, img->texture, NULL, &pos, _camera->_uniform_rotation, NULL, SDL_FLIP_NONE);
			SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
		}
		else {
			SDL_RenderCopyEx(_renderer, img->texture, NULL, &get_des(pos), _camera->_uniform_rotation, NULL, SDL_FLIP_NONE);
		}
	}
	else {
		if (ui_element) {
			SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
			draw_rect(pos, NULL_TEXTURE_COLOR);
			SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
		}
		else {
			draw_rect(get_des(pos), NULL_TEXTURE_COLOR);
		}
	}
}

void Renderer::render(const Texture *img, PositionComponent *position) {
	if (img != nullptr) {
		SDL_RenderCopyEx(_renderer, img->texture, NULL, &get_des(position->rect), _camera->_uniform_rotation + position->rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		draw_rect(get_des(position->rect), NULL_TEXTURE_COLOR);
	}
}

void Renderer::render(Sprite *img, SpriteComponent *sprite, PositionComponent *position) {
	if (img != nullptr) {
		sprite->update(img);
		SDL_RenderCopyEx(_renderer, img->texture, &sprite->pos, &get_des(position->rect), _camera->_uniform_rotation + position->rotation, NULL, SDL_FLIP_NONE);
	}
	else {
		draw_rect(get_des(position->rect), NULL_TEXTURE_COLOR);
	}
}

void Renderer::draw_text(Text &text, bool ui_element) {
	if (!text.loaded) {
		create_text(text);
	}
	if (text.texture != nullptr) {
		if (ui_element) {
			SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
			SDL_RenderCopy(_renderer, text.texture, NULL, &text.rect);
			SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
		}
		else {
			SDL_RenderCopyEx(_renderer, text.texture, NULL, &get_des(text.rect), _camera->_uniform_rotation, NULL, SDL_FLIP_NONE);
		}
	}
	else {
		if (ui_element) {
			SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
			draw_rect(text.rect, NULL_TEXT_COLOR);
			SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
		}
		else {
			draw_rect(get_des(text.rect), NULL_TEXT_COLOR);
		}
	}
}

// flags
// DRAW_RECT_EMPTY // doesnt not fill rectangle
// DRAW_RECT_CAMERA // applys camera
void Renderer::draw_rect(const SDL_Rect &rect, const SDL_Color &color, int flag) {
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	if (flag & DRAW_RECT_CAMERA) {
		SDL_RenderDrawRect(_renderer, &get_des(rect));
		if (!(flag & DRAW_RECT_EMPTY)) {
			SDL_RenderFillRect(_renderer, &get_des(rect));
		}
	}
	else {
		SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
		SDL_RenderDrawRect(_renderer, &rect);
		if (!(flag & DRAW_RECT_EMPTY)) {
			SDL_RenderFillRect(_renderer, &rect);
		}
		SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
	}
}

// rect.x - x1
// rect.y - y1
// rect.w - x2
// rect.h - y2
void Renderer::draw_line(const SDL_Rect &rect, const SDL_Color &color) {
	SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderSetScale(_renderer, 1.0f, 1.0f);
	SDL_RenderDrawLine(_renderer, rect.x, rect.y, rect.w, rect.h);
	SDL_RenderSetScale(_renderer, _camera->_uniform_scale, _camera->_uniform_scale);
}


SDL_Texture *Renderer::make_blit_texture(std::map<int, SDL_Surface *> &surfaces, const int &width, const int &height, const int &surface_size) {
	SDL_Surface *surface = SDL_CreateRGBSurface(NULL, width, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_Rect pos = { 0, 0, surface_size, surface_size };
	int i = 0;
	for (auto it = surfaces.begin(); it != surfaces.end(); ++it, ++i) {
		pos.x = int(i * surface_size);
		it->second->clip_rect.w = surface_size;
		it->second->clip_rect.h = surface_size;
		if (pos.x >= width) {
			pos.x = 0;
			pos.y += surface_size;
			i = 0;
		}
		SDL_BlitScaled(it->second, NULL, surface, &pos);
	}

	return SDL_CreateTextureFromSurface(_renderer, surface);
}

SDL_Texture *Renderer::blit_texture(SDL_Surface *&main_surface, SDL_Surface *surface, SDL_Rect &pos) {
	SDL_BlitSurface(surface, NULL, main_surface, &pos);
	return SDL_CreateTextureFromSurface(_renderer, main_surface);
}

void Renderer::create_text(Text &text) {
	if (text.text == "") {
		return;
	}
	SDL_Surface *surface = TTF_RenderText_Blended_Wrapped(_font, text.text.c_str(), text.color, text.wrap_length);
	text.texture = SDL_CreateTextureFromSurface(_renderer, surface);
	text.rect.w = int((float)surface->clip_rect.w * ((float)text.font_size / (float)FONT_PT_SIZE));
	text.rect.h = int((float)surface->clip_rect.h * ((float)text.font_size / (float)FONT_PT_SIZE));
	text.rect.x -= text.rect.w / TEXT_OFFSET_FACTOR_X;
	text.rect.y -= text.rect.h / TEXT_OFFSET_FACTOR_Y;
	SDL_FreeSurface(surface);
	text.loaded = true;
}

SDL_Rect Renderer::get_des(const SDL_Rect &pos) {
	return {
		pos.x - (int)_camera->_x,
		pos.y - (int)_camera->_y,
		pos.w,
		pos.h
	};
}