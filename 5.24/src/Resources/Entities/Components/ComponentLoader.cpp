#include "ComponentLoader.h"

#include <iostream>
#include <string>

#include "FileReader.h"

#include "Component.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "SpellComponent.h"
#include "MagicComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

#include "Environment.h"
#include "Log.h"

#include "Globals.h"

#define FILE_POSITION_COMPONENT "Position"
#define FILE_SPRITE_COMPONENT "Sprite"
#define FILE_SPELL_COMPONENT "Spell"
#define FILE_MAGIC_COMPONENT "Magic"
#define FILE_PLAYER_COMPONENT "Player"
#define FILE_ENEMY_COMPONENT "Enemy"

#define FILE_POSITION_WIDTH "iwidth"
#define FILE_POSITION_HEIGHT "iheight"
#define FILE_POSITION_SPEED "fspeed"

#define FILE_SPRITE_WIDTH "iwidth"
#define FILE_SPRITE_HEIGHT "iheight"
#define FILE_SPRITE_TIME "isprite_time"

#define FILE_SPELL_NAME "sspell_name"
#define FILE_SPELL_MAX_DIS "fmax_dis"
#define FILE_SPELL_SPEED "fspell_speed"
#define FILE_SPELL_DEATH_TIME "ideath_time"
#define FILE_SPELL_SCRIPT "sspell_script"

#define FILE_MAGIC_MAIN_SPELL_ID "imain_spell_id"
#define FILE_MAGIC_CAST_SPEED "icast_speed"

#define FILE_ENEMY_NAME "senemy_name"
#define FILE_ENEMY_SCRIPT "senemy_script"

void load_position(FileReader &file, Entity *entity, PositionComponent *&position) {
	int w = 32, h = 32;
	float speed = 0.0;

	if (file.exists(FILE_POSITION_WIDTH))  w = file.get_int(FILE_POSITION_WIDTH); 
	if (file.exists(FILE_POSITION_HEIGHT))  h = file.get_int(FILE_POSITION_HEIGHT); 
	if (file.exists(FILE_POSITION_SPEED))  speed = file.get_float(FILE_POSITION_SPEED); 

	position = new PositionComponent(entity, 0.0, 0.0, w, h, speed);
}

void load_sprite(FileReader &file, Entity *entity, SpriteComponent *&sprite) {
	int w = 0, h = 0, time = -1;

	if (file.exists(FILE_SPRITE_WIDTH))  w = file.get_int(FILE_SPRITE_WIDTH); 
	if (file.exists(FILE_SPRITE_HEIGHT))  h = file.get_int(FILE_SPRITE_HEIGHT); 
	if (file.exists(FILE_SPRITE_TIME))  time = file.get_int(FILE_SPRITE_TIME); 

	sprite = new SpriteComponent(entity, w, h, time);
}

void load_spell(FileReader &file, Entity *entity, SpellComponent *&spell) {
	float max_dis = 0;
	float speed = 0;
	int death_time = 0;
	std::string script = " ";
	std::string name = " ";

	if (file.exists(FILE_SPELL_NAME)) name = file.get_string(FILE_SPELL_NAME);
	if (file.exists(FILE_SPELL_MAX_DIS)) max_dis = file.get_float(FILE_SPELL_MAX_DIS);
	if (file.exists(FILE_SPELL_SPEED)) speed = file.get_float(FILE_SPELL_SPEED);
	if (file.exists(FILE_SPELL_DEATH_TIME)) death_time = file.get_int(FILE_SPELL_DEATH_TIME);
	if (file.exists(FILE_SPELL_SCRIPT)) script = file.get_string(FILE_SPELL_SCRIPT);

	spell = new SpellComponent(entity, name, max_dis, speed, death_time, script);
}

void load_magic(FileReader &file, Entity *entity, MagicComponent *&magic) {
	int main_spell_id = 0;
	int cast_speed = 0;

	if (file.exists(FILE_MAGIC_MAIN_SPELL_ID)) main_spell_id = file.get_int(FILE_MAGIC_MAIN_SPELL_ID);
	if (file.exists(FILE_MAGIC_CAST_SPEED)) cast_speed = file.get_int(FILE_MAGIC_CAST_SPEED);

	magic = new MagicComponent(entity, main_spell_id, cast_speed);
}

void load_player(FileReader &file, Entity *entity, PlayerComponent *&player) {
	player = new PlayerComponent(entity);
}

void load_enemy(FileReader &file, Entity *entity, EnemyComponent *&enemy) {
	std::string name = " ";
	std::string script = " ";

	if (file.exists(FILE_ENEMY_NAME)) name = file.get_string(FILE_ENEMY_NAME);
	if (file.exists(FILE_ENEMY_SCRIPT)) script = file.get_string(FILE_ENEMY_SCRIPT);

	enemy = new EnemyComponent(entity, name, script);
}

bool load_components(Entity *entity) {
	/*
	Environment::get().get_log()->print(
		"Loading Entity with type: \n "
		+ entity->get_type()
		+ "\n type_id: "
		+ std::to_string(entity->get_type_id())
	);
	*/

	std::string locate_file_path = ENTITY_FOLDER + entity->get_type() + "/" + entity->get_type() + ".txt";
	FileReader locate_file(locate_file_path.c_str());
	std::string entity_type_id = std::to_string(entity->get_type_id());

	if (!locate_file.exists(entity_type_id)) {
		Environment::get().get_log()->print(
			"No entity with type_id of: \""
			+ std::to_string(entity->get_type_id())
			+ "\" exits."
		);
		return false;
	}

	FileReader file(locate_file.get_string(entity_type_id).c_str());

	int numComponents = 0;
	entity->clear();

	//Environment::get().get_log()->print("Loading Components: ", "\n", false);

	if (file.exists(FILE_POSITION_COMPONENT)) {
	//	Environment::get().get_log()->print("Position ", "\n", false);
		PositionComponent *position = nullptr;
		load_position(file, entity, position);
		entity->add_component(position);
		numComponents++;
	}

	if (file.exists(FILE_SPRITE_COMPONENT)) {
	//	Environment::get().get_log()->print("Sprite ", "\n", false);
		SpriteComponent *sprite = nullptr;
		load_sprite(file, entity, sprite);
		entity->add_component(sprite);
		numComponents++;
	}

	if (file.exists(FILE_SPELL_COMPONENT)) {
	//	Environment::get().get_log()->print("Spell ", "\n", false);
		SpellComponent *spell = nullptr;
		load_spell(file, entity, spell);
		entity->add_component(spell);
		numComponents++;
	}

	if (file.exists(FILE_MAGIC_COMPONENT)) {
	//	Environment::get().get_log()->print("Magic ", "\n", false);
		MagicComponent *magic = nullptr;
		load_magic(file, entity, magic);
		entity->add_component(magic);
		numComponents++;
	}

	if (file.exists(FILE_PLAYER_COMPONENT)) {
		PlayerComponent *player = nullptr;
		load_player(file, entity, player);
		entity->add_component(player);
		numComponents++;
	}

	if (file.exists(FILE_ENEMY_COMPONENT)) {
		EnemyComponent *enemy = nullptr;
		load_enemy(file, entity, enemy);
		entity->add_component(enemy);
		numComponents++;
	}

	//Environment::get().get_log()->print("- " + std::to_string(numComponents) + " Component(s).", "\n", false);

	return true;
}