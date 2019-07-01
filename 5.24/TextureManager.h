#include <memory>
#include <map>

#include <SDL.h>

#include "Texture.h"
#include "Sprite.h"
#include "Entity.h"
#include "Map.h"

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

extern const char *TEXTURE_BASE_PATH;

namespace tmanager {
	typedef std::string Type;
	typedef int Key;
}

using namespace tmanager;

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void load_textures(Type type, std::string path);
	Texture *load_texture_info_tile(std::string path);
	Texture *load_texture_info(std::string path);

	void load_surfaces(Type type, std::string path);
	SDL_Surface *load_surface_info(std::string path);

	void load_map_texture(std::vector<Map::Tile> &tiles, const int &width, const int &height);
	void update_map_texture(SDL_Rect &pos, int id);

	const Texture *get_texture_info(Type type, Key key) { return _textures[type][key]; }
	Texture *get_texture_info(Entity *entity) { return _textures[entity->get_type()][entity->get_type_id()]; }
	SDL_Surface *get_surface_info(Type type, Key key) { return _surfaces[type][key]; }
    Sprite *get_sprite_info(Entity *entity) { return static_cast<Sprite *>(_textures[entity->get_type()][entity->get_type_id()]); }

	unsigned int get_texture_size(Type type) { return _textures[type].size(); }
	unsigned int get_surface_size(Type type) { return _surfaces[type].size(); }

	friend class ResourceManager;
private:
	SDL_Texture *make_map_blit_texture(SDL_Surface *&main_surface, std::map<int, SDL_Surface *> &surfaces, std::vector<Map::Tile> &tiles, const int &width, const int &height);
	SDL_Texture *make_editor_line_background(const int &width, const int &height, const int &tile_width, const int &tile_height, const SDL_Color &color);
private:
	typedef std::map<int, SDL_Surface *> Surface_Map;
	std::map<Type, Surface_Map> _surfaces;

	typedef std::map<Key, Texture *> Texture_Map;
	std::map<Type, Texture_Map> _textures;

	Texture *_map_texture;
	SDL_Surface *_map_surface;

	SDL_Surface *_editor_tiles_surface;
	Texture *_editor_tiles_texture;

	SDL_Surface *_editor_objects_surface;
	Texture *_editor_objects_texture;

	Texture *_editor_line_background;
};

#endif