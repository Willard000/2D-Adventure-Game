
#include "EntityManager.h"
#include "TextureManager.h"
#include "Map.h"
#include "UIManager.h"

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

class ResourceManager : 
	public EntityManager,
	public TextureManager
{
public:
	ResourceManager();
	~ResourceManager();
	
	void update();
	void render_entities();

	void render_entity(Entity *entity);
	void render_map();

	void render();

	bool load_map(int id);
	void edit_map(int index, int id);

	void render_editor(const UI::Element_Area &element_area, const UI::Element &placement);

	Map *get_map() { return _map; }
private:
	Map *_map;
};

#endif