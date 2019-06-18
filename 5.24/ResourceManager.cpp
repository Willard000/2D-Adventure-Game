#include "ResourceManager.h"

#include "Environment.h"
#include "WindowManager.h"
#include "LogManager.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

ResourceManager::ResourceManager() :
	_entityManager			( new EntityManager () ),
	_textureManager		    ( new TextureManager() ),
	_map					( nullptr )
{
	Environment::get().getLogManager()->log("Loading Resource Manager");
	loadMap(0);
}

ResourceManager::~ResourceManager() {
	Environment::get().getLogManager()->log("Closing Resource Manager");

	delete _entityManager;
	delete _textureManager;
}

void ResourceManager::update() {
	_entityManager->update();
	Environment::get().getWindowManager()->getWindow()->getCamera()->update(); // move this
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
		Environment::get().getWindowManager()->getRenderer()->render(_textureManager->getSpriteInfo(entity), sprite, position);
	}
	else {
		Environment::get().getWindowManager()->getRenderer()->render(_textureManager->getTextureInfo(entity), position);
	}
}

void ResourceManager::renderMap() {
	Environment::get().getWindowManager()->getRenderer()->render(_textureManager->_map, _map->_pos);
}

void ResourceManager::render() {
	renderMap();
	renderEntities();
}

void ResourceManager::loadMap(int id) {
	if (!_map) {
		_map = new Map(id);
	}
	else {
		_map->load(id);
	}
	_textureManager->loadMap(_map->_tiles, _map->_pos.w, _map->_pos.h);
}