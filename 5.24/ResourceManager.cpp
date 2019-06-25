#include "ResourceManager.h"

#include "Environment.h"
#include "Window.h"
#include "Log.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

ResourceManager::ResourceManager() :
	_entityManager			( new EntityManager () ),
	_textureManager		    ( new TextureManager() ),
	_map					( new Map() )
{
	Environment::get().getLog()->print("Loading Resource Manager");

	loadMap(0);
}

ResourceManager::~ResourceManager() {
	Environment::get().getLog()->print("Closing Resource Manager");

	delete _entityManager;
	delete _textureManager;
}

void ResourceManager::update() {
	_entityManager->update();
	Environment::get().getWindow()->getCamera()->update(); // move this
}

void ResourceManager::renderEntities() {
	renderEntity(_entityManager->_player);

	for (auto it = _entityManager->_entities.begin(); it != _entityManager->_entities.end(); it++) {
		renderEntity(it->second);
	}
}

void ResourceManager::renderEntity(Entity *entity) {
	PositionComponent *position = GetPosition(entity);
	SpriteComponent *sprite = GetSprite(entity);
	if (!position) {
		return;
	}

	if (sprite) {
		Environment::get().getWindow()->getRenderer()->render(_textureManager->getSpriteInfo(entity), sprite, position);
	}
	else {
		Environment::get().getWindow()->getRenderer()->render(_textureManager->getTextureInfo(entity), position);
	}
}

void ResourceManager::renderMap() {
	Environment::get().getWindow()->getRenderer()->render(_textureManager->_map, _map->_pos);
}

void ResourceManager::renderEditor(const UI::Element_Area &element_area) {
	Environment::get().getWindow()->getRenderer()->drawRect(element_area.background, element_area.color, true);
	Environment::get().getWindow()->getRenderer()->render(_textureManager->_editor_tiles_texture, element_area.element_area, true);
}

void ResourceManager::render() {
	renderMap();
	renderEntities();
}

void ResourceManager::loadMap(int id) {
	_map->load(id);
	_textureManager->loadMap(_map->_tiles, _map->_pos.w, _map->_pos.h);
}

void ResourceManager::editMap(int index, int id) {
	if (_map->_tiles[index].id == id) {
		return;
	}
	_map->_tiles[index].id = id;
	_textureManager->_map->texture = Environment::get().getWindow()->getRenderer()->blitTexture(
		_textureManager->_map_surface,
		_textureManager->getSurfaceInfo(TYPE_TILE, id),
		_map->_tiles[index].pos
	);
}