#include <memory>
#include <map>

#include <SDL.h>

#include "FileReader.h"
#include "Window.h"
#include "Texture.h"
#include "Entity.h"

#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

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

extern const char *TEXTURE_BASE_PATH;

namespace txm {
	typedef std::string Type;
	typedef int Key;
}

class TextureManager {
public:
	TextureManager(std::shared_ptr<Renderer> renderer);
	~TextureManager();

	void loadTextures(txm::Type type, std::string path);
	Texture *loadTextureInfo(std::string path);

	const Texture *getTextureInfo(txm::Type type, txm::Key key) { return _textures[type][key]; }
    Texture *getTextureInfo(Entity *entity) { return _textures[entity->get_type()][entity->get_type_id()]; }

	friend class ResourceManager;
private:
	std::shared_ptr<Renderer> _renderer;

	typedef std::map<txm::Key, Texture *> Texture_Map;
	std::map<txm::Type, Texture_Map> _textures;
};

#endif