#include "TextureManager.h"

const char *TEXTURE_BASE_PATH = "Data/Textures/textures.txt";

TextureManager::TextureManager(std::shared_ptr<Renderer> renderer) {
	_renderer = renderer;

	FileReader file(TEXTURE_BASE_PATH);

	for (freader::iterator it = file.begin(); it != file.end(); it++) {
		loadTextures(it->first, it->second);
	}
}

TextureManager::~TextureManager() {
	for (auto it = _textures.begin(); it != _textures.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			delete itt->second;
		}
	}
	_textures.clear();
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
	Texture *img = new Texture();
	FileReader file(path.c_str());

	if (file.exists(FILE_TEXTURE_PATH)) img->texture = _renderer->createTexture(file.get_string(FILE_TEXTURE_PATH));
	if (file.exists(FILE_TEXTURE_W)) img->rect.w = file.get_int(FILE_TEXTURE_W);
	if (file.exists(FILE_TEXTURE_H)) img->rect.h = file.get_int(FILE_TEXTURE_H);
	if (file.exists(FILE_FRAME_RUN)) img->sprite.run = file.get_int(FILE_FRAME_RUN);
	if (file.exists(FILE_FRAME_CAST)) img->sprite.cast = file.get_int(FILE_FRAME_CAST);
	if (file.exists(FILE_FRAME_SPECIAL)) img->sprite.special = file.get_int(FILE_FRAME_SPECIAL);
	if (file.exists(FILE_FRAME_MIN_UP)) img->sprite.min_up = file.get_int(FILE_FRAME_MIN_UP);
	if (file.exists(FILE_FRAME_MAX_UP)) img->sprite.max_up = file.get_int(FILE_FRAME_MAX_UP);
	if (file.exists(FILE_FRAME_MIN_DOWN)) img->sprite.min_down = file.get_int(FILE_FRAME_MIN_DOWN);
	if (file.exists(FILE_FRAME_MAX_DOWN)) img->sprite.max_down = file.get_int(FILE_FRAME_MAX_DOWN);
	if (file.exists(FILE_FRAME_MIN_LEFT)) img->sprite.min_left = file.get_int(FILE_FRAME_MIN_LEFT);
	if (file.exists(FILE_FRAME_MAX_LEFT)) img->sprite.max_left = file.get_int(FILE_FRAME_MAX_LEFT);
	if (file.exists(FILE_FRAME_MIN_RIGHT)) img->sprite.min_right = file.get_int(FILE_FRAME_MIN_RIGHT);
	if (file.exists(FILE_FRAME_MAX_RIGHT)) img->sprite.max_right = file.get_int(FILE_FRAME_MAX_RIGHT);

	return img;
}