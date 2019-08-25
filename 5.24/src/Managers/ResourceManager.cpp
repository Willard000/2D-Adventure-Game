#include "ResourceManager.h"

#include "Environment.h"
#include "Window.h"
#include "Clock.h"
#include "Log.h"

#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "CombatComponent.h"
#include "EnemyComponent.h"

#include "PlayerLoader.h"

bool SHOW_COMBAT_RANGE = false;			// DEBUG
bool SHOW_PATHING = false;

#define SOLIDS_COLOR {255, 0, 50, 50}
#define WARPS_COLOR {0, 255, 50, 50}

ResourceManager::ResourceManager() :
	TextureManager(),
	EntityManager(),
	TextManager(),
	_map(new Map())
{
	Environment::get().get_log()->print("Loading Resource Manager");
}

ResourceManager::~ResourceManager() {
	Environment::get().get_log()->print("Closing Resource Manager");
}

void ResourceManager::update() {
	update_entities();
	update_text();
	Environment::get().get_window()->get_camera()->update();
	_map->update();
}

void ResourceManager::render_entities() {
	for (auto &object : _entities[TYPE_OBJECT]) {
		render_entity(object.second);
	}

	for (auto &item : _entities[TYPE_ITEM]) {
		render_entity(item.second);
	}

	for (auto &enemy : _entities[TYPE_ENEMY]) {
		render_entity(enemy.second);
	}

	render_entity(_player);

	for (auto &spell : _entities[TYPE_SPELL]) {
		render_entity(spell.second);
	}

	for (auto &effect : _entities[TYPE_EFFECT]) {
		render_entity(effect.second);
	}
}

void ResourceManager::render_entity(Entity *entity) {
	PositionComponent *position = GetPosition(entity);
	SpriteComponent *sprite = GetSprite(entity);
	CombatComponent *combat = GetCombat(entity);

	if (!position) {
		return;
	}

	if (sprite) {
		Environment::get().get_window()->get_renderer()->render(get_sprite_info(entity), sprite, position);
	}
	else {
		Environment::get().get_window()->get_renderer()->render(get_texture_info(entity), position);
	}

	if (combat) {
		combat->draw_health();
	}

	if (SHOW_COMBAT_RANGE) {
		EnemyComponent *enemy = GetEnemy(entity);
		if (enemy) {
			SDL_Rect range = { position->rect.x - enemy->combat_range.w / 2, position->rect.y - enemy->combat_range.h / 2, enemy->combat_range.w, enemy->combat_range.h };
			Environment::get().get_window()->get_renderer()->draw_rect(range, { 0, 255, 0, 150 }, DRAW_RECT_CAMERA);
		}
	}

	if (SHOW_PATHING) {
		EnemyComponent *enemy = GetEnemy(entity);
		if (enemy) {
			for (auto &p : enemy->pathing) {
				SDL_Rect path_rect = { p.x - PATH_WIDTH / 2, p.y - PATH_HEIGHT / 2, PATH_WIDTH, PATH_HEIGHT };
				Environment::get().get_window()->get_renderer()->draw_rect(path_rect, { 0, 0, 255, 150 }, DRAW_RECT_CAMERA);
			}
			SDL_Rect center_rect = { position->rect.x - PATH_WIDTH / 2 + position->rect.w / 2, position->rect.y - PATH_HEIGHT / 2 + position->rect.h / 2, PATH_WIDTH, PATH_HEIGHT };
			Environment::get().get_window()->get_renderer()->draw_rect(center_rect, { 255, 150, 0, 150 }, DRAW_RECT_CAMERA);
		}
	}

}

void ResourceManager::render_map() {
	if (Environment::get().get_mode() == MODE_GAME) {
		Environment::get().get_window()->get_renderer()->render(_map_texture, _map->_rect);
	}
	else {
		for (auto &v : _map_textures) {
			for (auto &t : v) {
				Environment::get().get_window()->get_renderer()->render(t);
			}
		}
	}
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
	else if (selection.type == TYPE_ITEM)
		renderer->render(_editor_items_texture, element_area.area, true);

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
	render_text();
}

bool ResourceManager::load_map(int id, bool is_base_map) {
	if (!_map->load(id, is_base_map)) {
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