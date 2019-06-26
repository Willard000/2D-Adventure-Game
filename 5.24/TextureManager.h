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

	void loadTextures(Type type, std::string path);
	Texture *loadTextureInfoTile(std::string path);
	Texture *loadTextureInfo(std::string path);

	void loadSurfaces(Type type, std::string path);
	SDL_Surface *loadSurfaceInfo(std::string path);

	void loadMap(std::vector<Map::Tile> &tiles, const int &width, const int &height);
	void updateMap(SDL_Rect &pos, int id);

	const Texture *getTextureInfo(Type type, Key key) { return _textures[type][key]; }
	Texture *getTextureInfo(Entity *entity) { return _textures[entity->get_type()][entity->get_type_id()]; }
	SDL_Surface *getSurfaceInfo(Type type, Key key) { return _surfaces[type][key]; }
    Sprite *getSpriteInfo(Entity *entity) { return static_cast<Sprite *>(_textures[entity->get_type()][entity->get_type_id()]); }

	unsigned int getTextureSize(Type type) { return _textures[type].size(); }
	unsigned int getSurfaceSize(Type type) { return _surfaces[type].size(); }

	friend class ResourceManager;
private:
	SDL_Texture *makeMapBlitTexture(SDL_Surface *&main_surface, std::map<int, SDL_Surface *> &surfaces, std::vector<Map::Tile> &tiles, const int &width, const int &height);
private:
	typedef std::map<int, SDL_Surface *> Surface_Map;
	std::map<Type, Surface_Map> _surfaces;

	typedef std::map<Key, Texture *> Texture_Map;
	std::map<Type, Texture_Map> _textures;

	Texture *_map;
	SDL_Surface *_map_surface;

	SDL_Surface *_editor_tiles_surface;
	Texture *_editor_tiles_texture;
};

#endif