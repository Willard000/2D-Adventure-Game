#include "ResourceManager.h"

#include "Environment.h"
#include "Window.h"
#include "Clock.h"
#include "Log.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"

#define SOLIDS_COLOR {255, 0, 50, 50}
#define WARPS_COLOR {0, 255, 50, 50}

ResourceManager::ResourceManager() :
	TextureManager(),
	EntityManager(),
	_map(new Map())
{
	Environment::get().get_log()->print("Loading Resource Manager");
}

ResourceManager::~ResourceManager() {
	Environment::get().get_log()->print("Closing Resource Manager");
}

void ResourceManager::update() {
	EntityManager::update();
	Environment::get().get_window()->get_camera()->update(); // move this
	_map->update();
}

void ResourceManager::render_entities() {
	for (auto it = _entities.begin(); it != _entities.end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			render_entity(itt->second);
		}
	}

	render_entity(_player);
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

void ResourceManager::render_editor(const UI::Element_Area &element_area, const UI::Element &selection) {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	for (auto it = _map->_solids.begin(); it != _map->_solids.end(); ++it) {
		renderer->draw_rect(it->second, SOLIDS_COLOR, DRAW_RECT_CAMERA);
	}

	for (auto it = _map->_warps.begin(); it != _map->_warps.end(); ++it) {
		renderer->draw_rect(it->from, WARPS_COLOR, DRAW_RECT_CAMERA);
	}

	renderer->render(_editor_line_background);

	renderer->draw_rect(element_area.background, element_area.color);

	if (selection.type == TYPE_TILE)
		renderer->render(_editor_tiles_texture, element_area.area, true);
	else if (selection.type == TYPE_OBJECT)
		renderer->render(_editor_objects_texture, element_area.area, true);
	else if (selection.type == TYPE_ENEMY)
		renderer->render(_editor_enemies_texture, element_area.area, true);
	else if (selection.type == TYPE_EFFECT)
		renderer->render(_editor_effects_texture, element_area.area, true);

	if (selection.id != -1) {
		if (selection.type == TYPE_ENEMY || selection.type == TYPE_EFFECT) {
			renderer->render(get_texture_info(selection.type + TYPE_EX_ICON, selection.id), element_area.info, true);
		}
		else {
			renderer->render(get_texture_info(selection.type, selection.id), element_area.info, true);
		}
	}
}

void ResourceManager::render() {
	render_map();
	render_entities();
}

bool ResourceManager::load_map(int id) {
	if (!_map->load(id)) {
		return false;
	}
	load_map_texture(_map->_tiles, _map->_rect.w, _map->_rect.h);

	Environment::get().get_clock()->reset();

	return true;
}

void ResourceManager::edit_map(int index, int id) {
	if (index < 0 || index > (int)_map->_tiles.size() - 1 ||
		_map->_tiles[index].id == id ) {
		return;
	}
	_map->_tiles[index].id = id;
	update_map_texture(_map->_tiles[index].pos, id);
}