#include "TextureManager.h"

#include "Environment.h"
#include "Window.h"
#include "Log.h"
#include "UIManager.h"

#include "FileReader.h"

#include "Globals.h"

#define FILE_SPRITE "Sprite"
#define FILE_TEXTURE_PATH "stexture"
#define FILE_TEXTURE_W "iwidth"
#define FILE_TEXTURE_H "iheight"
#define FILE_FRAME_IDLE "iidle"
#define FILE_FRAME_END "iend"
#define FILE_FRAME_RUN "irun"
#define FILE_FRAME_CAST "icast"
#define FILE_FRAME_SPECIAL "ispecial"
#define FILE_FRAME_MIN_IDLE "imin_idle"
#define FILE_FRAME_MAX_IDLE "imax_idle"
#define FILE_FRAME_MIN_UP "imin_up"
#define FILE_FRAME_MAX_UP "imax_up"
#define FILE_FRAME_MIN_DOWN "imin_down"
#define FILE_FRAME_MAX_DOWN "imax_down"
#define FILE_FRAME_MIN_LEFT "imin_left"
#define FILE_FRAME_MAX_LEFT "imax_left"
#define FILE_FRAME_MIN_RIGHT "imin_right"
#define FILE_FRAME_MAX_RIGHT "imax_right"
#define FILE_FRAME_NUM_FRAMES "inum_frames"

#define EDITOR_LINE_COLOR {255, 255, 255, 50}

#define MAP_TEXTURE_SECTION_SIZE 10

TextureManager::TextureManager() :
	_map_texture				( new Texture() ),
	_map_surface				( nullptr ),
	_editor_tiles_texture	    ( nullptr ),
	_editor_objects_texture		( nullptr ),
	_editor_enemies_texture     ( nullptr ),
	_editor_effects_texture		( nullptr ),
	_editor_items_texture		( nullptr ),
	_editor_npcs_texture		( nullptr ),
	_editor_line_background		( nullptr )
{
	Environment::get().get_log()->print("Loading Texture Manager");

	Environment::get().get_log()->print("Loading Textures");
	FileReader texture_file(TEXTURE_FILE);
	for (freader::iterator it = texture_file.begin(); it != texture_file.end(); ++it) {
		Environment::get().get_log()->print("Texture Type & Path: " + it->first + " " + it->second);
		load_textures(stoi(it->first), it->second);
	}
	Environment::get().get_log()->print("Finished loading Textures");

	Environment::get().get_log()->print("Loading Surfaces");
	FileReader surface_file(SURFACE_FILE);
	for (freader::iterator it = surface_file.begin(); it != surface_file.end(); ++it) {
		load_surfaces(stoi(it->first), it->second);
	}
	Environment::get().get_log()->print("Finished loading Surfaces");

	if (Environment::get().get_mode() == MODE_EDITOR)
		load_editor_textures();
}

TextureManager::~TextureManager() {
	Environment::get().get_log()->print("Closing Texture Manager");

	for (auto &texture_vec : _textures) {
		for (auto &texture : texture_vec) {
			delete texture;
		}
		texture_vec.clear();
	}

	for (auto &surface_vec : _surfaces) {
		for (auto &surface : surface_vec) {
			SDL_FreeSurface(surface);
		}
		surface_vec.clear();
	}

	delete _map_texture;
	SDL_FreeSurface(_map_surface);

	for (auto &v : _map_surfaces) {
		for (auto &s : v) {
			SDL_FreeSurface(s);
		}
	}

	for (auto &v : _map_textures) {
		for (auto &t : v) {
			delete t;
		}
	}

	delete _editor_tiles_texture;
	delete _editor_objects_texture;
	delete _editor_enemies_texture;
	delete _editor_effects_texture;
	delete _editor_items_texture;
	delete _editor_npcs_texture;

	delete _editor_line_background;
}

void TextureManager::load_textures(const Type &type, const std::string &path) {
	std::vector<Texture *> textures;
	FileReader file(path.c_str());

	textures.resize(file.size());
	for (freader::iterator it = file.begin(); it != file.end(); ++it) {
		if (type == TYPE_TILE) {
			textures[std::stoi(it->first)] = load_texture_info_tile(it->second);
		}
		else {
			textures[std::stoi(it->first)] = load_texture_info(it->second);
		}
	}

	_textures[type] = textures;
}

Texture *TextureManager::load_texture_info_tile(std::string path) {
	Environment::get().get_log()->print("Loading Texture Tile - " + path);

	Texture *img = new Texture();
	img->texture = Environment::get().get_window()->get_renderer()->create_texture(path);
	img->rect.w = TILE_WIDTH;
	img->rect.h = TILE_HEIGHT;
	return img;
}

Texture *TextureManager::load_texture_info(std::string path) {
	Environment::get().get_log()->print("Loading Texture - " + path);

	Texture *img = nullptr;
	Sprite *sprite = nullptr;
	FileReader file(path.c_str());
	bool isSprite = file.exists(FILE_SPRITE);

	if (!isSprite) {
		img = new Texture();
		if (file.exists(FILE_TEXTURE_PATH)) img->texture = Environment::get().get_window()->get_renderer()->create_texture(file.get_string(FILE_TEXTURE_PATH));
		if (file.exists(FILE_TEXTURE_W)) img->rect.w = file.get_int(FILE_TEXTURE_W);
		if (file.exists(FILE_TEXTURE_H)) img->rect.h = file.get_int(FILE_TEXTURE_H);
	}

	if (isSprite) {
		sprite = new Sprite();
		if (file.exists(FILE_TEXTURE_PATH)) sprite->texture = Environment::get().get_window()->get_renderer()->create_texture(file.get_string(FILE_TEXTURE_PATH));
		if (file.exists(FILE_TEXTURE_W)) sprite->rect.w = file.get_int(FILE_TEXTURE_W);
		if (file.exists(FILE_TEXTURE_H)) sprite->rect.h = file.get_int(FILE_TEXTURE_H);
		if (file.exists(FILE_FRAME_IDLE)) sprite->idle = file.get_int(FILE_FRAME_IDLE);
		if (file.exists(FILE_FRAME_END)) sprite->end = file.get_int(FILE_FRAME_END);
		if (file.exists(FILE_FRAME_RUN)) sprite->run = file.get_int(FILE_FRAME_RUN);
		if (file.exists(FILE_FRAME_CAST)) sprite->cast = file.get_int(FILE_FRAME_CAST);
		if (file.exists(FILE_FRAME_SPECIAL)) sprite->special = file.get_int(FILE_FRAME_SPECIAL);
		if (file.exists(FILE_FRAME_MIN_IDLE)) sprite->min_idle = file.get_int(FILE_FRAME_MIN_IDLE);
		if (file.exists(FILE_FRAME_MAX_IDLE)) sprite->max_idle = file.get_int(FILE_FRAME_MAX_IDLE);
		if (file.exists(FILE_FRAME_MIN_UP)) sprite->min_up = file.get_int(FILE_FRAME_MIN_UP);
		if (file.exists(FILE_FRAME_MAX_UP)) sprite->max_up = file.get_int(FILE_FRAME_MAX_UP);
		if (file.exists(FILE_FRAME_MIN_DOWN)) sprite->min_down = file.get_int(FILE_FRAME_MIN_DOWN);
		if (file.exists(FILE_FRAME_MAX_DOWN)) sprite->max_down = file.get_int(FILE_FRAME_MAX_DOWN);
		if (file.exists(FILE_FRAME_MIN_LEFT)) sprite->min_left = file.get_int(FILE_FRAME_MIN_LEFT);
		if (file.exists(FILE_FRAME_MAX_LEFT)) sprite->max_left = file.get_int(FILE_FRAME_MAX_LEFT);
		if (file.exists(FILE_FRAME_MIN_RIGHT)) sprite->min_right = file.get_int(FILE_FRAME_MIN_RIGHT);
		if (file.exists(FILE_FRAME_MAX_RIGHT)) sprite->max_right = file.get_int(FILE_FRAME_MAX_RIGHT);
		if (file.exists(FILE_FRAME_NUM_FRAMES)) sprite->num_frames = file.get_int(FILE_FRAME_NUM_FRAMES);

		sprite->load_frames();
	}

	return isSprite ? sprite : img;
}

void TextureManager::load_surfaces(const Type &type, const std::string &path) {
	std::vector<SDL_Surface *> surfaces;
	FileReader file(path.c_str());

	surfaces.resize(file.size(), nullptr);
	for (freader::iterator it = file.begin(); it != file.end(); ++it) {
		surfaces[std::stoi(it->first)] = load_surface_info(it->second);
	}

	_surfaces[type] = surfaces;
}

SDL_Surface *TextureManager::load_surface_info(const std::string &path) {
	Environment::get().get_log()->print("Loading Surface - " + path);

	FileReader file(path.c_str());
	SDL_Surface *surface = Environment::get().get_window()->get_renderer()->create_surface(path);

	return surface;
}

void TextureManager::load_map_texture(std::vector<Map::Tile> &tiles, const int &width, const int &height) {
	if (Environment::get().get_mode() == MODE_GAME) {
		SDL_DestroyTexture(_map_texture->texture);
		_map_texture->texture = make_map_blit_texture(_map_surface, _surfaces[TYPE_TILE], tiles, width, height);
		return;
	}

	// editor

	_editor_line_background->texture = make_editor_line_background(width, height, TILE_WIDTH, TILE_HEIGHT, EDITOR_LINE_COLOR);
	_editor_line_background->rect.w = width;
	_editor_line_background->rect.h = height;

	for (auto &v : _map_surfaces) {
		for (auto &s : v) {
			SDL_FreeSurface(s);
		}
	}
	_map_surfaces.clear();

	for (auto &v : _map_textures) {
		for (auto &t : v) {
			delete t;
		}
	}
	_map_textures.clear();

	int rows = (width / TILE_WIDTH) / MAP_TEXTURE_SECTION_SIZE;
	int columns = (height / TILE_HEIGHT) / MAP_TEXTURE_SECTION_SIZE;

	for (int i = 0; i < columns; ++i) {
		std::vector<Texture *> texture_row;
		std::vector<SDL_Surface *> surface_row;
		for (int k = 0; k < rows; ++k) {
			SDL_Surface *surface = SDL_CreateRGBSurface(NULL, MAP_TEXTURE_SECTION_SIZE * TILE_WIDTH, MAP_TEXTURE_SECTION_SIZE * TILE_HEIGHT, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
			Texture * section = new Texture();

			int section_start_index = k * MAP_TEXTURE_SECTION_SIZE + (i * rows * MAP_TEXTURE_SECTION_SIZE * MAP_TEXTURE_SECTION_SIZE); 

			for (int h = 0; h < MAP_TEXTURE_SECTION_SIZE; ++h) {
				for (int w = 0; w < MAP_TEXTURE_SECTION_SIZE; ++w) {
					int index = w + (h * rows * MAP_TEXTURE_SECTION_SIZE) + section_start_index;
					SDL_Rect blit_rect = { w * TILE_WIDTH, h * TILE_HEIGHT, 0, 0 };
					SDL_BlitSurface(_surfaces[TYPE_TILE][tiles[index].id], NULL, surface, &blit_rect);
				}
			}

			section->texture = SDL_CreateTextureFromSurface(Environment::get().get_window()->get_renderer()->get_renderer(), surface);
			section->rect = {
				k * MAP_TEXTURE_SECTION_SIZE * TILE_WIDTH,
				i * MAP_TEXTURE_SECTION_SIZE * TILE_HEIGHT,
				MAP_TEXTURE_SECTION_SIZE * TILE_WIDTH,
				MAP_TEXTURE_SECTION_SIZE * TILE_HEIGHT
			};
			texture_row.push_back(section);
			surface_row.push_back(surface);
		}
		_map_textures.push_back(texture_row);
		_map_surfaces.push_back(surface_row);
	}
}

void TextureManager::update_map_texture(SDL_Rect &pos, int id) {
	if (Environment::get().get_mode() == MODE_GAME) {
		SDL_DestroyTexture(_map_texture->texture);

		_map_texture->texture = Environment::get().get_window()->get_renderer()->blit_texture(
			_map_surface,
			get_surface_info(TYPE_TILE, id),
			pos
		);

		return;
	}

	// editor

	int x_index = pos.x / TILE_WIDTH / MAP_TEXTURE_SECTION_SIZE;
	int y_index = pos.y / TILE_HEIGHT / MAP_TEXTURE_SECTION_SIZE;

	SDL_DestroyTexture(_map_textures[y_index][x_index]->texture);

	SDL_Rect tile_pos = { pos.x - (TILE_WIDTH * MAP_TEXTURE_SECTION_SIZE) * x_index, pos.y - (TILE_HEIGHT * MAP_TEXTURE_SECTION_SIZE) * y_index, NULL, NULL };

	_map_textures[y_index][x_index]->texture = Environment::get().get_window()->get_renderer()->blit_texture(
		_map_surfaces[y_index][x_index],
		get_surface_info(TYPE_TILE, id),
		tile_pos
	);
}

SDL_Texture *TextureManager::make_map_blit_texture(SDL_Surface *&main_surface, std::vector<SDL_Surface *> &surfaces, std::vector<Map::Tile> &tiles, const int &width, const int &height) {
	if (main_surface) {
		SDL_FreeSurface(main_surface);
	}
	main_surface = SDL_CreateRGBSurface(NULL, width, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);

	for (unsigned int i = 0; i < tiles.size(); ++i) {
		SDL_BlitSurface(surfaces[tiles[i].id], NULL, main_surface, &tiles[i].pos);
	}

	return SDL_CreateTextureFromSurface(Environment::get().get_window()->get_renderer()->get_renderer(), main_surface);
}

SDL_Texture *TextureManager::make_editor_line_background(const int &width, const int &height, const int &tile_width, const int &tile_height, const SDL_Color &color) {
	SDL_Surface *surface = SDL_CreateRGBSurface(NULL, width, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);

	SDL_Surface *line = SDL_CreateRGBSurface(NULL, 1, height, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_FillRect(line, NULL, SDL_MapRGB(line->format, color.r, color.g, color.b));

	SDL_Rect pos = { 0, 0, 0,0 };
	for (int i = 0; i <= width; ++i) {
		pos.x = int(i * tile_width);
		SDL_BlitSurface(line, NULL, surface, &pos);
	}
	SDL_FreeSurface(line);

	line = SDL_CreateRGBSurface(NULL, width, 1, RGB_DEPTH, RMASK, GMASK, BMASK, AMASK);
	SDL_FillRect(line, NULL, SDL_MapRGB(line->format, color.r, color.g, color.b));

	pos = { 0, 0, 0, 0 };
	for (int i = 0; i <= height; ++i) {
		pos.y = int(i * tile_height);
		SDL_BlitSurface(line, NULL, surface, &pos);
	}
	SDL_FreeSurface(line);

	SDL_SetSurfaceAlphaMod(surface, color.a);

	return SDL_CreateTextureFromSurface(Environment::get().get_window()->get_renderer()->get_renderer(), surface);
}

void TextureManager::load_editor_textures() {
	_editor_line_background = new Texture();

	_editor_tiles_texture = new Texture();
	_editor_tiles_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_TILE],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);

	_editor_objects_texture = new Texture();
	_editor_objects_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_OBJECT],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);

	_editor_enemies_texture = new Texture();
	_editor_enemies_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_ENEMY],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);

	_editor_effects_texture = new Texture();
	_editor_effects_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_EFFECT],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);

	_editor_items_texture = new Texture();
	_editor_items_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_ITEM],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);

	_editor_npcs_texture = new Texture();
	_editor_npcs_texture->texture = Environment::get().get_window()->get_renderer()->make_blit_texture(
		_surfaces[TYPE_NPC],
		ELEMENT_AREA_WIDTH,
		Environment::get().get_window()->get_height(),
		TILE_WIDTH
	);
}