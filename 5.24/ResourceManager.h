
#include "EntityManager.h"
#include "TextureManager.h"
#include "System.h"
#include "Map.h"

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {
public:
	ResourceManager(std::shared_ptr<System> system);
	
	void update();
	void renderEntities();

	void renderEntity(Entity *entity);
	void renderMap();

	void render();

	void loadMap(int id);

	Entity *getEntity(int id) { return _entityManager->get(id); }
	Player *getPlayer()       { return _entityManager->getPlayer(); }
private:
	std::shared_ptr<EntityManager> _entityManager;
	std::shared_ptr<TextureManager> _textureManager;
	std::shared_ptr<System> _system;
	std::shared_ptr<Renderer> _renderer;
	Map *_map;
};

#endif