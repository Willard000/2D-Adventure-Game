#include <memory>
#include <map>

#include <SDL.h>

#include "Texture.h"
#include "Sprite.h"
#include "Entity.h"
#include "Map.h"

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#define FILE_SPRITE "Sprite"
#define FILE_TEXTURE_PATH "stexture"
#define FILE_TEXTURE_W "iwidth"
#define FILE_TEXTURE_H "iheight"
#define FILE_FRAME_RUN "irun"
#define FILE_FRAME_CAST "icast"
#define FILE_FRAME_SPECIAL "ispecial"
#define FILE_FRAME_MIN_UP "imin_up"
#define FILE_FRAME_MAX_UP "imax_up"
#define FILE_FRAME_MIN_DOWN "imin_down"
#define FILE_FRAME_MAX_DOWN "imax_down"
#define FILE_FRAME_MIN_LEFT "imin_left"
#define FILE_FRAME_MAX_LEFT "imax_left"
#define FILE_FRAME_MIN_RIGHT "imin_right"
#define FILE_FRAME_MAX_RIGHT "imax_right"
#define FILE_FRAME_NUM_FRAMES "inum_frames"

extern const char *TEXTURE_BASE_PATH;

namespace tmanager {
	typedef std::string Type;
	typedef int Key;
}

class TextureManager {
public:
	TextureManager();
	~TextureManager();

	void loadTextures(tmanager::Type type, std::string path);
	Texture *loadTextureInfo(std::string path);

	void loadSurfaces(tmanager::Type type, std::string path);
	SDL_Surface *loadSurfaceInfo(std::string path);

	void loadMap(std::vector<Tile> &tiles, const int &width, const int &height);

	const Texture *getTextureInfo(tmanager::Type type, tmanager::Key key) { return _textures[type][key]; }
	Texture *getTextureInfo(Entity *entity) { return _textures[entity->get_type()][entity->get_type_id()]; }
    Sprite *getSpriteInfo(Entity *entity) { return static_cast<Sprite *>(_textures[entity->get_type()][entity->get_type_id()]); }

	friend class ResourceManager;
private:
	typedef std::map<int, SDL_Surface *> Surface_Map;
	std::map<tmanager::Type, Surface_Map> _surfaces;

	typedef std::map<tmanager::Key, Texture *> Texture_Map;
	std::map<tmanager::Type, Texture_Map> _textures;

	Texture *_map;
};

#endif