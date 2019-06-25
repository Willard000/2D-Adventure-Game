
#include "EntityManager.h"
#include "TextureManager.h"
#include "Map.h"
#include "UIManager.h"

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
	void editMap(int index, int id);

	void renderEditor(const UI::Element_Area &element_area);

	Entity *getEntity(int id) { return _entityManager->get(id); }
	Player *getPlayer()       { return _entityManager->getPlayer(); }
	Map *getMap() { return _map; }

	unsigned int getSurfaceSize(std::string type) { return _textureManager->getSurfaceSize(type); }
private:
	EntityManager *_entityManager;
	TextureManager *_textureManager;
	Map *_map;
};

#endif