#include "UIManager.h"

#include "Environment.h"
#include "ResourceManager.h"
#include "Window.h"
#include "Log.h"
#include "Clock.h"
#include "InputManager.h"

#include "Collision.h"

#include "UIHandler.h"
#include "Button_Pressable.h"

#include "Timer.h"

#include "PositionComponent.h"
#include "EnemyComponent.h"

#define MOUSE_WIDTH 1
#define MOUSE_HEIGHT 1

#define BUTTON_CONFIRM "Yes"
#define BUTTON_CONFIRM_XOFFSET 260
#define BUTTON_CONFIRM_YOFFSET 60
#define BUTTON_CONFIRM_W 120
#define BUTTON_CONFIRM_H 30
#define BUTTON_CONFIRM_FTSIZE 24
#define BUTTON_DENY "No"

#define CURRENT_TEXT_COLOR { 255, 255, 255, 255 }
#define CURRENT_TEXT_FTSIZE 24
#define CURRENT_TEXT_YOFFSET 200

#define MOUSE_TEXT_TIME 100
#define MOUSE_TEXT_XOFFSET 50

#define ALIGNMENT_TEXT "Alignment: "
#define ALIGNMENT_TEXT_XOFFSET 200

#define SELECTION_TEXT "Selection: "
#define SELECTION_TEXT_XOFFSET 500

#define INFO_TEXT_COLOR {255, 255, 255, 255}
#define INFO_TEXT_FTSIZE 14
#define INFO_TEXT_WRAP_SIZE 1000
#define INFO_TEXT_YOFFSET -20

#define ELEMENT_SELECTION_COLOR { 0, 250, 240, 50 }
#define ELEMENT_SELECTION_HEIGHT 256
#define ELEMENT_AREA_COLOR { 100, 100, 100, 100 }

#define WARP_COLOR {0, 255, 0, 100}

#define PATH_COLOR { 0, 100, 225, 150 }

#define HIGHLIGHT_FACTOR 80

SDL_Rect get_mouse_rect(const int &x, const int &y) {
	return { x, y, MOUSE_WIDTH, MOUSE_HEIGHT };
}

bool element_collision(const int &x, const int &y) {
	SDL_Rect placement = get_mouse_rect(x, y);

	std::map<int, EntityManager::Entity_Map> *entities = Environment::get().get_resource_manager()->get_entities();
	for (auto it = entities->begin(); it != entities->end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			if (PositionComponent *position = GetPosition(itt->second)) {
				if (collision(placement, position->rect)) {
					return true;
				}
			}
		}
	}

	return false;
}

UIManager::UIManager() :
	_state					( STATE_IDLE ),
	_element_area			( { {Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								 ELEMENT_AREA_COLOR,
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT, ELEMENT_AREA_WIDTH, Environment::get().get_window()->get_height()},
								{Environment::get().get_window()->get_width() - ELEMENT_AREA_WIDTH, 0, ELEMENT_AREA_WIDTH, ELEMENT_SELECTION_HEIGHT} } ),
	_selection				( { TYPE_TILE, -1 } ),
	_map_selection			( { TYPE_OBJECT, -1 } ),
	_align_placement		( true ),
	_mouse_button			( 0 ),
	_mouse_x				( 0 ),
	_mouse_y				( 0 ),
	_on_confirm				( nullptr ),
	_on_deny				( nullptr ),
	_highlighted_button		( nullptr ),
	_current_text           ( "", CURRENT_TEXT_COLOR, CURRENT_TEXT_FTSIZE, 0,
						   	  Environment::get().get_window()->get_width_half(),
							  Environment::get().get_window()->get_height_half() - CURRENT_TEXT_YOFFSET ),
	_alignment_text			( ALIGNMENT_TEXT + std::string("On"), INFO_TEXT_COLOR, INFO_TEXT_FTSIZE, INFO_TEXT_WRAP_SIZE, ALIGNMENT_TEXT_XOFFSET, Environment::get().get_window()->get_height() + INFO_TEXT_YOFFSET ),
	_selection_text         ( SELECTION_TEXT + STYPE(TYPE_TILE), INFO_TEXT_COLOR, INFO_TEXT_FTSIZE, INFO_TEXT_WRAP_SIZE, SELECTION_TEXT_XOFFSET, Environment::get().get_window()->get_height() + INFO_TEXT_YOFFSET )
{
	Environment::get().get_log()->print("Loading UI Manager");
}

UIManager::~UIManager() {
	Environment::get().get_log()->print("Closing UI Manager");

	for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
		delete it->second;
	}
}

void UIManager::add_button(Button *button) {
	_buttons[button->get_text().get_text()] = button;
}

void UIManager::remove_button(std::string key) {
	if (_buttons[key]) {
		delete _buttons[key];
		_buttons.erase(key);
		return;
	}

	Environment::get().get_log()->print("Couldn't remove button at key - " + key);
}

int UIManager::update() {
	SDL_GetMouseState(&_mouse_x, &_mouse_y);
	update_mouse_location();
	highlight_button();

	if (_state == STATE_CONFIRM || _state == STATE_DENY) {
		pop_confirmation();

		if (_state == STATE_CONFIRM && _on_confirm) {
			_state = STATE_WAITING;
			_on_confirm();
			_on_confirm = nullptr;  
		}
		else if (_state == STATE_DENY && _on_deny) {
			_state = STATE_WAITING;
			_on_deny();
			_on_deny = nullptr;
		}

		_state = _state_prev;
	}

	return _state;
}

void UIManager::update_mouse_location() {
	static Timer mouse_update(MOUSE_TEXT_TIME);

	if (mouse_update.update()) {
		int x, y;
		calc_real_mouse_location(x, y);
		x = x / TILE_WIDTH + 1, y = y / TILE_HEIGHT + 1;

		_mouse_location = Text(
			"x: " + std::to_string(x) + " y: " + std::to_string(y),
			INFO_TEXT_COLOR,
			INFO_TEXT_FTSIZE,
			INFO_TEXT_WRAP_SIZE,
			MOUSE_TEXT_XOFFSET,
			Environment::get().get_window()->get_height() + INFO_TEXT_YOFFSET
		);
	}
}

void UIManager::highlight_button() {
	for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
		if (collision(it->second->get_rect(), get_mouse_rect(_mouse_x, _mouse_y))) {
			if (_highlighted_button == it->second) {
				return;
			}

			if (_highlighted_button) {
				SDL_Color color = _highlighted_button->get_color();
				color.a -= HIGHLIGHT_FACTOR;
				_highlighted_button->set_color(color);
			}

			SDL_Color color = it->second->get_color();
			color.a += HIGHLIGHT_FACTOR;
			it->second->set_color(color);
			_highlighted_button = it->second;
			return;
		}
	}

	if (_highlighted_button) {
		SDL_Color color = _highlighted_button->get_color();
		color.a -= HIGHLIGHT_FACTOR;
		_highlighted_button->set_color(color);
	}

	_highlighted_button = nullptr;
}

bool UIManager::check_buttons() {
	for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
		if (collision(it->second->get_rect(), get_mouse_rect(_mouse_x, _mouse_y))) {
			it->second->execute();
			return true;
		}
	}

	return false;
}

bool UIManager::check_selection(int mouse_button) {
	_mouse_button = mouse_button;

	if (_mouse_x >= _element_area.area.x) {
		_selection.id = select();
		if (_selection.id != -1) {
			_state = STATE_PLACING;
		}
		else {
			_state = STATE_IDLE;
		}
		return true;
	}

	if (_state == STATE_PLACING) {
		place_on_map();
		return true;
	}

	if (_state == STATE_SELECTING) {
		_map_selection = select_from_map();
		return true;
	}

	return false;
}

void UIManager::render() {
	Renderer *renderer = Environment::get().get_window()->get_renderer();

	Environment::get().get_resource_manager()->render_editor(_element_area, _selection);
	if (_state == STATE_WAITING)
		renderer->draw_text(&_current_text, true);
	else if (_state == STATE_PLACING) {
		if (_selection.id != -1) {
			int x, y;
			calc_real_mouse_location(x, y);
			Texture *img = nullptr;
			if (_selection.type == TYPE_ENEMY || _selection.type == TYPE_EFFECT)
				img = Environment::get().get_resource_manager()->get_texture_info(_selection.type + TYPE_EX_ICON, _selection.id);
			else
				img = Environment::get().get_resource_manager()->get_texture_info(_selection.type, _selection.id);

			if (img != nullptr) {
				renderer->render(img, { (int)x, (int)y, TILE_WIDTH, TILE_HEIGHT });
			}
		}
	}
	else if (_state == STATE_SELECTING && _map_selection.id != -1) {
		if (_map_selection.type != TYPE_WARP) {
			Entity *entity = Environment::get().get_resource_manager()->get_entity(_map_selection.type, _map_selection.id);
			if (PositionComponent *position = GetPosition(entity)) {
				renderer->draw_rect(position->rect, ELEMENT_SELECTION_COLOR, DRAW_RECT_CAMERA);
			}
		}
		else if (_map_selection.type == TYPE_WARP) {
			renderer->draw_rect((*Environment::get().get_resource_manager()->get_map()->get_warps())[_map_selection.id].from, ELEMENT_SELECTION_COLOR, DRAW_RECT_CAMERA);
		}
	}

	for (auto it = _buttons.begin(); it != _buttons.end(); ++it) {
		renderer->draw_rect(it->second->get_rect(), it->second->get_color());
		renderer->draw_text(&it->second->get_text(), true);
	}


	renderer->draw_text(&_mouse_location, true);
	renderer->draw_text(&_alignment_text, true);
	renderer->draw_text(&_selection_text, true);
}

void UIManager::set_state(int flag) {
	_state = flag;
}

void UIManager::set_current_text(std::string text) {
	_current_text.set_text(text);
}

void UIManager::push_confirmation(Callback on_confirm, Callback on_deny) {
	_on_confirm = on_confirm;
	_on_deny = on_deny;
	int width = Environment::get().get_window()->get_width();
	int height = Environment::get().get_window()->get_height();
	Button_Pressable *button_confirm = new Button_Pressable(
		&confirm_yes,
		BUTTON_CONFIRM,
		{ (width / 2) - BUTTON_CONFIRM_XOFFSET, (height / 2) - BUTTON_CONFIRM_YOFFSET, BUTTON_CONFIRM_W, BUTTON_CONFIRM_H },
		BUTTON_CONFIRM_FTSIZE
	);

	Button_Pressable *button_deny = new Button_Pressable(
		&confirm_no,
		BUTTON_DENY,
		{ (width / 2) + BUTTON_CONFIRM_XOFFSET - BUTTON_CONFIRM_W, (height / 2) - BUTTON_CONFIRM_YOFFSET, BUTTON_CONFIRM_W, BUTTON_CONFIRM_H },
		BUTTON_CONFIRM_FTSIZE
	);

	add_button(button_confirm);
	add_button(button_deny);

	_state_prev = _state;
	_state = STATE_WAITING;
}

void UIManager::pop_confirmation() {
	remove_button(BUTTON_CONFIRM);
	remove_button(BUTTON_DENY);
}

int UIManager::select() {
	if (_state == STATE_WAITING) {
		return -1;
	}
	if (_mouse_y < _element_area.area.y) {
		return -1;
	}
	int index = int((_mouse_x - _element_area.area.x) / ELEMENT_WIDTH) + int(((_mouse_y - _element_area.area.y) / ELEMENT_HEIGHT) * ELEMENT_ROW_SIZE);

	if (index < 0 || index > (int)Environment::get().get_resource_manager()->get_surface_size(_selection.type) - 1) {
		return -1;
	}

	return index;
}

bool UIManager::place_on_map() {
	if (_state == STATE_WAITING) {
		return false;
	}
	SDL_Rect boundry = Environment::get().get_resource_manager()->get_map()->get_rect();
	int x, y;
	calc_real_mouse_location(x, y);

	if (x < 0 || x > boundry.x + boundry.w || y < 0 || y > boundry.y + boundry.h) {
		return false;
	}

	int index = int(x / TILE_WIDTH) + ((int(y / TILE_HEIGHT) * int(boundry.w / TILE_WIDTH)));

	if (_selection.type == TYPE_TILE) {
		Environment::get().get_resource_manager()->edit_map(index, _selection.id);
		return true;
	}

	if (_selection.type == TYPE_SOLID) {
		if(_mouse_button == MOUSE_LEFT)
			Environment::get().get_resource_manager()->get_map()->add_solid(index);
		else 
			Environment::get().get_resource_manager()->get_map()->remove_solid(index);
		return true;
	}

	if (_selection.type == TYPE_WARP) {
		place_warp();
		return true;
	}

	if (_align_placement) {
		int map_width = Environment::get().get_resource_manager()->get_map()->get_width();
		x = (index % map_width) * TILE_WIDTH + (TILE_WIDTH / 2);
		y = (index / map_width) * TILE_HEIGHT + (TILE_HEIGHT / 2);
	}

	if (element_collision((int)x, (int)y)) {
		return false;
	}

	if (_selection.type == TYPE_ENEMY) {
		place_enemy((float)x, (float)y);
		return true;
	}

	if (_selection.type == TYPE_OBJECT || _selection.type == TYPE_EFFECT || _selection.type == TYPE_ITEM) {
		Environment::get().get_resource_manager()->create_entity(_selection.type, _selection.id, (float)x, (float)y);
	}

	return true;
}

void UIManager::place_warp() {
	Map::Warp warp;
	Map *map = Environment::get().get_resource_manager()->get_map();
	int map_id = map->get_id();

	warp.from_id = map_id;
	warp.from = place_warp_rect();

	set_current_text("Enter Warp Map ID");
	Environment::get().get_input_manager()->get_text_input(&warp.to_id);

	map->save(true);
	map->save(false);

	if (!Environment::get().get_resource_manager()->load_map(warp.to_id, true)) {
		return;
	}

	warp.to = place_warp_rect();

	Environment::get().get_resource_manager()->load_map(map_id, true);

	map->add_warp(warp);

	_state = STATE_IDLE;
}

SDL_Rect UIManager::place_warp_rect() {
	_state = STATE_WAITING;
	set_current_text("Select Warp Start Point");

	bool end_input = false;
	bool start_point = true;
	bool end_point = false;

	SDL_Rect pos = { 0, 0, 0, 0 };

	int x, y;
	while (!end_point && (Environment::get().get_input_manager()->get())) {
		Environment::get().get_window()->get_renderer()->clear();

		Environment::get().get_ui_manager()->update();

		Environment::get().get_input_manager()->update_editor_camera();

		if ((Environment::get().get_input_manager()->is_mouse(SDL_BUTTON_LEFT))) {
			if (start_point) {
				calc_real_mouse_location(pos.x, pos.y);
				start_point = false;
				set_current_text("Select Warp End Point");
			}
			else {
				end_point = true;
			}
		}

		Environment::get().get_resource_manager()->render();
		Environment::get().get_ui_manager()->render();

		if (!start_point) {
			calc_real_mouse_location(x, y);
			pos.w = x - pos.x;
			pos.h = y - pos.y;

			Environment::get().get_window()->get_renderer()->draw_rect(pos, WARP_COLOR, DRAW_RECT_CAMERA);
		}

		Environment::get().get_window()->get_renderer()->render();

		if (Environment::get().get_clock()->update()) {
			std::string title = "Warp      Map: " + std::to_string(Environment::get().get_resource_manager()->get_map()->get_id()) + "     " +
				Environment::get().get_clock()->get_display_time() + "    " + std::to_string(Environment::get().get_clock()->get_fms());
			Environment::get().get_window()->set_title(title);
		}
	}

	if (pos.w < 0) {
		pos.x += pos.w;
		pos.w = abs(pos.w);
	}
	if (pos.h < 0) {
		pos.y += pos.h;
		pos.h = abs(pos.h);
	}
	 
	return pos;
}

void UIManager::place_enemy(float x, float y) {
	Entity *entity = new Entity(_selection.type, _selection.id);
	if (PositionComponent *position = GetPosition(entity)) {
		position->set(x - (position->rect.w / 2), y - (position->rect.h / 2));
	}

	EnemyComponent *enemy = GetEnemy(entity);
	if (!enemy) {
		Environment::get().get_resource_manager()->add_entity(entity);
		return;
	}

	std::vector<Path> pathing_;
	while (Environment::get().get_input_manager()->get()) {
		Environment::get().get_window()->get_renderer()->clear();

		Environment::get().get_ui_manager()->update();

		Environment::get().get_input_manager()->update_editor_camera();

		if (Environment::get().get_input_manager()->is_key(SDL_SCANCODE_SPACE)) {
			enemy->pathing = pathing_;
			break;
		}

		if (Environment::get().get_input_manager()->is_mouse(SDL_BUTTON_LEFT)) {
			int mouse_x, mouse_y;
			calc_real_mouse_location(mouse_x, mouse_y);

			if (_align_placement) {
				calc_align_mouse_location(mouse_x, mouse_y);
			}

			Path path;
			path.x = mouse_x;
			path.y = mouse_y;

			bool already_exists = false;
			for (auto &p : pathing_) {
				if (p.x == path.x && p.y == path.y) {
					already_exists = true;
					break;
				}
			}

			if (!already_exists) {
				pathing_.push_back(path);
			}
		}

		Environment::get().get_resource_manager()->render();
		Environment::get().get_resource_manager()->render_entity(entity);

		for (auto &p : pathing_) {
			Environment::get().get_window()->get_renderer()->draw_rect({ p.x - PATH_WIDTH / 2, p.y - PATH_HEIGHT / 2, PATH_WIDTH, PATH_HEIGHT }, PATH_COLOR, DRAW_RECT_CAMERA);
		}

		Environment::get().get_ui_manager()->render();

		Environment::get().get_window()->get_renderer()->render();

		if (Environment::get().get_clock()->update()) {
			std::string title = "Pathing      Map: " + std::to_string(Environment::get().get_resource_manager()->get_map()->get_id()) + "     " +
				Environment::get().get_clock()->get_display_time() + "    " + std::to_string(Environment::get().get_clock()->get_fms());
			Environment::get().get_window()->set_title(title);
		}
	}

	Environment::get().get_resource_manager()->add_entity(entity);
}

Element UIManager::select_from_map() {
	int x, y;
	calc_real_mouse_location(x, y);

	std::map<int, EntityManager::Entity_Map> *entities = Environment::get().get_resource_manager()->get_entities();
	SDL_Rect mouse = get_mouse_rect((int)x, (int)y);

	for (auto it = entities->begin(); it != entities->end(); ++it) {
		for (auto itt = it->second.begin(); itt != it->second.end(); ++itt) {
			if (PositionComponent *position = GetPosition(itt->second)) {
				if (collision(mouse, position->rect)) {
					return { itt->second->get_type(), itt->second->get_id() };
				}
			}
		}
	}

	std::vector<Map::Warp> *warps = Environment::get().get_resource_manager()->get_map()->get_warps();
	for (int i = 0; i < (int)warps->size(); ++i) {
		SDL_Rect warp_pos = (*warps)[i].from;
		if (collision(mouse, warp_pos)) {
			return { TYPE_WARP, i };
		}
	}

	return { 0 , -1 };
}

void UIManager::delete_map_selection() {
	if (_map_selection.id != -1) {
		if (_map_selection.type == TYPE_WARP) {
			Environment::get().get_resource_manager()->get_map()->remove_warp(_map_selection.id);
		}
		else {
			Environment::get().get_resource_manager()->remove_entity(_map_selection.type, _map_selection.id);
		}

		_map_selection.id = -1;
		_map_selection.type = 0;
	}
}

void UIManager::set_selection_type(Type type) {
	if (type == _selection.type) {
		return;
	}

	_last_selected_id[_selection.type] = _selection.id;
	if (_last_selected_id.find(type) == _last_selected_id.end())
		_selection.id = -1;
	else
		_selection.id = _last_selected_id[type];

	_selection.type = type;
	_selection_text.set_text(SELECTION_TEXT + STYPE(type));
}

Type UIManager::get_selection_type() {
	return _selection.type;
}

void UIManager::toggle_alignment() {
	_align_placement = !_align_placement;
	_alignment_text.set_text(_align_placement ? ALIGNMENT_TEXT + std::string("On") : ALIGNMENT_TEXT + std::string("Off"));
}

void UIManager::calc_real_mouse_location(int &x, int &y) {
	Camera *camera = Environment::get().get_window()->get_camera();
	x = int(double((_mouse_x / camera->get_scale()) + camera->get_x()));
	y = int(double((_mouse_y / camera->get_scale()) + camera->get_y()));
}

void UIManager::calc_align_mouse_location(int &x, int &y) {
	SDL_Rect boundry = Environment::get().get_resource_manager()->get_map()->get_rect();
	int index = int(x / TILE_WIDTH) + ((int(y / TILE_HEIGHT) * int(boundry.w / TILE_WIDTH)));
	int map_width = Environment::get().get_resource_manager()->get_map()->get_width();
	x = (index % map_width) * TILE_WIDTH + (TILE_WIDTH / 2);
	y = (index / map_width) * TILE_HEIGHT + (TILE_HEIGHT / 2);
}