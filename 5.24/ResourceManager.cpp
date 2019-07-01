#include "ResourceManager.h"

#include "Environment.h"
#include "Window.h"
#include "Log.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

ResourceManager::ResourceManager() :
	TextureManager(),
	EntityManager(),
	_map					( new Map() )
{
	Environment::get().get_log()->print("Loading Resource Manager");
}

ResourceManager::~ResourceManager() {
	Environment::get().get_log()->print("Closing Resource Manager");
}

void ResourceManager::update() {
	EntityManager::update();
	Environment::get().get_window()->get_camera()->update(); // move this
}

void ResourceManager::render_entities() {
	render_entity(_player);

	for (auto it = _entities.begin(); it != _entities.end(); it++) {
		render_entity(it->second);
	}
}

void ResourceManager::render_entity(Entity *entity) {
	PositionComponent *position = GetPosition(entity);
	SpriteComponent *sprite = GetSprite(entity);

	if (!position) {
		return;
	}

	if (sprite) {
		Environment::get().get_window()->get_renderer()->render(get_sprite_info(entity), sprite, position);
	}
	else {
		Environment::get().get_window()->get_renderer()->render(get_texture_info(entity), position);
	}
}

void ResourceManager::render_map() {
	Environment::get().get_window()->get_renderer()->render(_map_texture, _map->_rect);
}

void ResourceManager::render_editor(const UI::Element_Area &element_area, const UI::Element &placement) {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	renderer->render(_editor_line_background);
	
	renderer->draw_rect(element_area.background, element_area.color, true);

	if(placement.type == TYPE_TILE)
		renderer->render(_editor_tiles_texture, element_area.area, true);
	else if (placement.type == TYPE_OBJECT) 
		renderer->render(_editor_objects_texture, element_area.area, true);

	if (placement.id != -1) {
		renderer->render(get_texture_info(placement.type, placement.id), element_area.info, true);
	}
}

void ResourceManager::render() {
	render_map();
	render_entities();
}

void ResourceManager::load_map(int id) {
	_map->load(id);
	load_map_texture(_map->_tiles, _map->_rect.w, _map->_rect.h);
}

void ResourceManager::edit_map(int index, int id) {
	if (_map->_tiles[index].id == id || index < 0 || index > (int)_map->_tiles.size() - 1) {
		return;
	}
	_map->_tiles[index].id = id;
	update_map_texture(_map->_tiles[index].pos, id);
}