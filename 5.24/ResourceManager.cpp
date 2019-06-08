#include "ResourceManager.h"

ResourceManager::ResourceManager(std::shared_ptr<Renderer> renderer) {
	_entityManager = std::make_shared<EntityManager>();
	_textureManager = std::make_shared<TextureManager>(renderer);
	_renderer = renderer;
	_map = new Map(0);
	loadMap(0);
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
		_renderer->render(_textureManager->getSpriteInfo(entity), moveable->position, sprite);
		return;
	}

	if (moveable != nullptr) {
		_renderer->render(_textureManager->getTextureInfo(entity), moveable->position);
		return;
	}

	PositionComponent *position = GetPosition(entity);
	if (position != nullptr) {
		_renderer->render(_textureManager->getTextureInfo(entity), position->position);
	}
}

void ResourceManager::renderMap() {
	_renderer->render(_textureManager->_map, _map->_pos);
}

void ResourceManager::render() {
	_renderer->clear();
	_renderer->drawBackground();

	renderMap();
	renderEntities();

	_renderer->render();
}

void ResourceManager::loadMap(int id) {
	_map->load(id);
	_textureManager->loadMap(_map->_tiles, _map->_pos.w, _map->_pos.h);
}