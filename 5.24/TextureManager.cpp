#include "TextureManager.h"

const char *TEXTURE_BASE_PATH = "Data/Textures/textures.txt";
const char *SURFACE_BASE_PATH = "Data/Textures/surfaces.txt";

TextureManager::TextureManager(std::shared_ptr<Renderer> renderer) {
	_renderer = renderer;

	FileReader texture_file(TEXTURE_BASE_PATH);
	for (freader::iterator it = texture_file.begin(); it != texture_file.end(); it++) {
		loadTextures(it->first, it->second);
	}

	FileReader surface_file(SURFACE_BASE_PATH);
	for (freader::iterator it = surface_file.begin(); it != surface_file.end(); it++) {
		loadSurfaces(it->first, it->second);
	}

	_map = new Texture();
}

TextureManager::~TextureManager() {
	for (auto it = _textures.begin(); it != _textures.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			delete itt->second;
		}
	}
	_textures.clear();

	for (auto it = _surfaces.begin(); it != _surfaces.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			SDL_FreeSurface(itt->second);
		}
	}
	_surfaces.clear();

	delete _map;
}

void TextureManager::loadTextures(txm::Type type, std::string path) {
	Texture_Map textures;
	FileReader file(path.c_str());

	for (freader::iterator it = file.begin(); it != file.end(); it++) {
		textures[std::stoi(it->first)] = loadTextureInfo(it->second);
	}

	_textures[type] = textures;
}

Texture *TextureManager::loadTextureInfo(std::string path) {
	Texture *img = nullptr;
	Sprite *sprite = nullptr;
	FileReader file(path.c_str());
	bool isSprite = file.exists(FILE_SPRITE);

	if (!isSprite) {
		img = new Texture();
		if (file.exists(FILE_TEXTURE_PATH)) img->texture = _renderer->createTexture(file.get_string(FILE_TEXTURE_PATH));
		if (file.exists(FILE_TEXTURE_W)) img->rect.w = file.get_int(FILE_TEXTURE_W);
		if (file.exists(FILE_TEXTURE_H)) img->rect.h = file.get_int(FILE_TEXTURE_H);
	}

	if (isSprite) {
		sprite = new Sprite();
		if (file.exists(FILE_TEXTURE_PATH)) sprite->texture = _renderer->createTexture(file.get_string(FILE_TEXTURE_PATH));
		if (file.exists(FILE_TEXTURE_W)) sprite->rect.w = file.get_int(FILE_TEXTURE_W);
		if (file.exists(FILE_TEXTURE_H)) sprite->rect.h = file.get_int(FILE_TEXTURE_H);
		if (file.exists(FILE_FRAME_RUN)) sprite->run = file.get_int(FILE_FRAME_RUN);
		if (file.exists(FILE_FRAME_CAST)) sprite->cast = file.get_int(FILE_FRAME_CAST);
		if (file.exists(FILE_FRAME_SPECIAL)) sprite->special = file.get_int(FILE_FRAME_SPECIAL);
		if (file.exists(FILE_FRAME_MIN_UP)) sprite->min_up = file.get_int(FILE_FRAME_MIN_UP);
		if (file.exists(FILE_FRAME_MAX_UP)) sprite->max_up = file.get_int(FILE_FRAME_MAX_UP);
		if (file.exists(FILE_FRAME_MIN_DOWN)) sprite->min_down = file.get_int(FILE_FRAME_MIN_DOWN);
		if (file.exists(FILE_FRAME_MAX_DOWN)) sprite->max_down = file.get_int(FILE_FRAME_MAX_DOWN);
		if (file.exists(FILE_FRAME_MIN_LEFT)) sprite->min_left = file.get_int(FILE_FRAME_MIN_LEFT);
		if (file.exists(FILE_FRAME_MAX_LEFT)) sprite->max_left = file.get_int(FILE_FRAME_MAX_LEFT);
		if (file.exists(FILE_FRAME_MIN_RIGHT)) sprite->min_right = file.get_int(FILE_FRAME_MIN_RIGHT);
		if (file.exists(FILE_FRAME_MAX_RIGHT)) sprite->max_right = file.get_int(FILE_FRAME_MAX_RIGHT);
	}

	return isSprite ? sprite : img;
}

void TextureManager::loadSurfaces(txm::Type type, std::string path) {
	Surface_Map surfaces;
	FileReader file(path.c_str());

	for (freader::iterator it = file.begin(); it != file.end(); it++) {
		surfaces[std::stoi(it->first)] = loadSurfaceInfo(it->second);
	}

	_surfaces[type] = surfaces;
}

SDL_Surface *TextureManager::loadSurfaceInfo(std::string path) {
	SDL_Surface *surface = nullptr;
	FileReader file(path.c_str());

	if (file.exists(FILE_TEXTURE_PATH)) surface = _renderer->createSurface(file.get_string(FILE_TEXTURE_PATH));

	return surface;
}

void TextureManager::loadMap(std::vector<Tile> &tiles, const int &width, const int &height) {
	_map->texture = _renderer->createMapTexture(tiles, _surfaces[TYPE_TILE], width, height);
}