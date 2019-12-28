#include <array>

#include <SDL.h>

#include "Texture.h"
#include "Sprite.h"
#include "Entity.h"
#include "Map.h"

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#define SURFACE_ARRAY_SIZE 10
#define TEXTURE_ARRAY_SIZE 20

namespace tmanager {
	typedef int Type;
	typedef int Texture_ID;
}

using namespace tmanager;

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void load_textures(const Type &type, const std::string &path);
	Texture *load_texture_info_tile(std::string path);
	Texture *load_texture_info(std::string path);

	void load_surfaces(const Type &type, const std::string &path);
	SDL_Surface *load_surface_info(const std::string &path);

	void load_map_texture(std::vector<Map::Tile> &tiles, const int &width, const int &height);
	void update_map_texture(SDL_Rect &pos, int id);

	Texture *&get_texture_info(const Type &type, const Texture_ID &texture_id) { return _textures[type][texture_id]; }
	Texture *&get_texture_info(Entity *entity) { return _textures[entity->get_type()][entity->get_texture_id()]; }
	SDL_Surface *&get_surface_info(const Type &type, const Texture_ID &texture_id) { return _surfaces[type][texture_id]; }
	Sprite *get_sprite_info(const Type &type, const Texture_ID &texture_id) { return static_cast<Sprite *>(_textures[type][texture_id]); }
    Sprite *get_sprite_info(Entity *entity) { return static_cast<Sprite *>(_textures[entity->get_type()][entity->get_texture_id()]); }

	unsigned int get_texture_size(const Type &type) { return _textures[type].size(); }
	unsigned int get_surface_size(const Type &type) { return _surfaces[type].size(); }

	friend class ResourceManager;
private:
	SDL_Texture *make_map_blit_texture(SDL_Surface *&main_surface, std::vector<SDL_Surface *> &surfaces, std::vector<Map::Tile> &tiles, const int &width, const int &height);
	SDL_Texture *make_editor_line_background(const int &width, const int &height, const int &tile_width, const int &tile_height, const SDL_Color &color);
	void load_editor_textures();
private:
	std::array<std::vector<SDL_Surface *>, SURFACE_ARRAY_SIZE> _surfaces;

	std::array<std::vector<Texture *>, TEXTURE_ARRAY_SIZE> _textures;

	Texture *_map_texture;
	SDL_Surface *_map_surface;

	std::vector<std::vector<Texture *>> _map_textures;
	std::vector<std::vector<SDL_Surface *>> _map_surfaces;

	Texture *_editor_tiles_texture;
	Texture *_editor_objects_texture;
	Texture *_editor_enemies_texture;
	Texture *_editor_effects_texture;
	Texture *_editor_items_texture;
	Texture *_editor_npcs_texture;

	Texture *_editor_line_background;
};

#endif