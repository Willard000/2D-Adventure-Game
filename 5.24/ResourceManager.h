
#include "EntityManager.h"
#include "TextureManager.h"
#include "Map.h"

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager {
public:
	ResourceManager();
	~ResourceManager();
	
	void update();
	void renderEntities();

	void renderEntity(Entity *entity);
	void renderMap();

	void render();

	void loadMap(int id);

	Entity *getEntity(int id) { return _entityManager->get(id); }
	Player *getPlayer()       { return _entityManager->getPlayer(); }
	Map *getMap() { return _map; }
private:
	EntityManager *_entityManager;
	TextureManager *_textureManager;
	Map *_map;
};

#endif