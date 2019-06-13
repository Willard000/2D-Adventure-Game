#include "ResourceManager.h"

#include "Environment.h"
#include "WindowManager.h"

#include "PositionComponent.h"
#include "MoveableComponent.h"
#include "SpriteComponent.h"

ResourceManager::ResourceManager() {
	_entityManager = std::make_shared<EntityManager>();
	_textureManager = std::make_shared<TextureManager>();
	_map = new Map(0);
	loadMap(0);
}

void ResourceManager::update() {
	_entityManager->update();
	Environment::get().getWindowManager()->getWindow()->getCamera()->center(_entityManager->_player); // move this
}

void ResourceManager::renderEntities() {
	renderEntity(_entityManager->_player);

	for (auto it = _entityManager->_entities.begin(); it != _entityManager->_entities.end(); it++) {
		renderEntity(it->second);
	}
}

void ResourceManager::renderEntity(Entity *entity) {
	MoveableComponent *moveable = GetMoveable(entity);
	SpriteComponent *sprite = GetSprite(entity);

	if (sprite != nullptr && moveable != nullptr) {
		Environment::get().getWindowManager()->getRenderer()->render(_textureManager->getSpriteInfo(entity), moveable->position, sprite);
		return;
	}

	if (moveable != nullptr) {
		Environment::get().getWindowManager()->getRenderer()->render(_textureManager->getTextureInfo(entity), moveable->position);
		return;
	}

	PositionComponent *position = GetPosition(entity);
	if (position != nullptr) {
		Environment::get().getWindowManager()->getRenderer()->render(_textureManager->getTextureInfo(entity), position->position);
	}
}

void ResourceManager::renderMap() {
	Environment::get().getWindowManager()->getRenderer()->render(_textureManager->_map, _map->_pos);
}

void ResourceManager::render() {
	Environment::get().getWindowManager()->getRenderer()->clear();
	Environment::get().getWindowManager()->getRenderer()->drawBackground();

	renderMap();
	renderEntities();

	Environment::get().getWindowManager()->getWindow()->getRenderer()->render();
}

void ResourceManager::loadMap(int id) {
	_map->load(id);
	_textureManager->loadMap(_map->_tiles, _map->_pos.w, _map->_pos.h);
}