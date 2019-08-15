#include "ComponentLoader.h"

#include <iostream>
#include <string>
#include <sstream>

#include <SDL.h>

#include "FileReader.h"

#include "Component.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"
#include "SpellComponent.h"
#include "MagicComponent.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "EffectComponent.h"
#include "CombatComponent.h"
#include "ItemComponent.h"

#include "Environment.h"
#include "Log.h"

#include "Globals.h"

#define FILE_TEXTURE_ID "itexture_id"

#define FILE_POSITION_COMPONENT "Position"
#define FILE_SPRITE_COMPONENT "Sprite"
#define FILE_SPELL_COMPONENT "Spell"
#define FILE_MAGIC_COMPONENT "Magic"
#define FILE_PLAYER_COMPONENT "Player"
#define FILE_ENEMY_COMPONENT "Enemy"
#define FILE_EFFECT_COMPONENT "Effect"
#define FILE_COMBAT_COMPONENT "Combat"
#define FILE_ITEM_COMPONENT "Item"

#define FILE_POSITION_WIDTH "iwidth"
#define FILE_POSITION_HEIGHT "iheight"
#define FILE_POSITION_SPEED "fspeed"

#define FILE_SPRITE_WIDTH "iwidth"
#define FILE_SPRITE_HEIGHT "iheight"
#define FILE_SPRITE_TIME "isprite_time"
#define FILE_SPRITE_CAST_TIME "isprite_cast_time"

#define FILE_SPELL_MAX_DIS "fmax_dis"
#define FILE_SPELL_SPEED "fspell_speed"
#define FILE_SPELL_DEATH_TIME "ideath_time"
#define FILE_SPELL_SCRIPT_NAME "sspell_script_name"
#define FILE_SPELL_SCRIPT "sspell_script"
#define FILE_SPELL_DAMAGE "ispell_damage"
#define FILE_SPELL_COLOR "spell_color"

#define FILE_MAGIC_MAIN_SPELL_ID "imain_spell_id"
#define FILE_MAGIC_CAST_SPEED "icast_speed"

#define FILE_PLAYER_NAME "splayer_name"

#define FILE_ENEMY_NAME "senemy_name"
#define FILE_ENEMY_SCRIPT "senemy_script"
#define FILE_ENEMY_COMBAT_RANGEW "ienemy_combat_range_w"
#define FILE_ENEMY_COMBAT_RANGEH "ienemy_combat_range_h"

#define FILE_EFFECT_NAME "seffect_name"
#define FILE_EFFECT_SCRIPT "seffect_script"
#define FILE_EFFECT_RAND_ROTATION "ieffect_rand_rotation"
#define FILE_EFFECT_RAND_TIME "ieffect_rand_time"

#define FILE_COMBAT_MAX_HEALTH "icombat_max_health"
#define FILE_COMBAT_MAX_MANA "icombat_max_mana"
#define FILE_COMBAT_DAMAGE "icombat_damage"
#define FILE_COMBAT_ARMOR "icombat_armor"
#define FILE_COMBAT_HPS "icombat_hps"
#define FILE_COMBAT_MPS "icombat_mps"
#define FILE_COMBAT_TIME "icombat_time"

#define FILE_ITEM_SLOT "iitem_slot"
#define FILE_ITEM_NAME "sitem_name"
#define FILE_ITEM_HEALTH "iitem_health"
#define FILE_ITEM_MANA "iitem_mana"
#define FILE_ITEM_DAMAGE "iitem_damage"
#define FILE_ITEM_ARMOR "iitem_armor"
#define FILE_ITEM_HPS "iitem_hps"
#define FILE_ITEM_MPS "iitem_mps"
#define FILE_ITEM_EQUIPABLE "bitem_equipable"
#define FILE_ITEM_USEABLE "bitem_useable"

void load_position(FileReader &file, Entity *entity, PositionComponent *&position) {
	int w = 32, h = 32;
	float speed = 0.0;

	if (file.exists(FILE_POSITION_WIDTH))  w = file.get_int(FILE_POSITION_WIDTH); 
	if (file.exists(FILE_POSITION_HEIGHT))  h = file.get_int(FILE_POSITION_HEIGHT); 
	if (file.exists(FILE_POSITION_SPEED))  speed = file.get_float(FILE_POSITION_SPEED); 

	position = new PositionComponent(entity, 0.0, 0.0, w, h, speed);
}

void load_sprite(FileReader &file, Entity *entity, SpriteComponent *&sprite) {
	int w = 0, h = 0;
	int time = -1;
	int cast_time = -1;

	if (file.exists(FILE_SPRITE_WIDTH))  w = file.get_int(FILE_SPRITE_WIDTH); 
	if (file.exists(FILE_SPRITE_HEIGHT))  h = file.get_int(FILE_SPRITE_HEIGHT); 
	if (file.exists(FILE_SPRITE_TIME))  time = file.get_int(FILE_SPRITE_TIME); 
	if (file.exists(FILE_SPRITE_CAST_TIME)) cast_time = file.get_int(FILE_SPRITE_CAST_TIME);

	sprite = new SpriteComponent(entity, w, h, time, cast_time);
}

void load_spell(FileReader &file, Entity *entity, SpellComponent *&spell) {
	float max_dis = 0;
	float speed = 0;
	int death_time = 0;
	std::string script = " ";
	std::string script_name = " ";
	int damage = 0;
	SDL_Color color = { 0, 0, 0, 255 };

	if (file.exists(FILE_SPELL_MAX_DIS)) max_dis = file.get_float(FILE_SPELL_MAX_DIS);
	if (file.exists(FILE_SPELL_SPEED)) speed = file.get_float(FILE_SPELL_SPEED);
	if (file.exists(FILE_SPELL_DEATH_TIME)) death_time = file.get_int(FILE_SPELL_DEATH_TIME);
	if (file.exists(FILE_SPELL_SCRIPT_NAME)) script_name = file.get_string(FILE_SPELL_SCRIPT_NAME);
	if (file.exists(FILE_SPELL_SCRIPT)) script = file.get_string(FILE_SPELL_SCRIPT);
	if (file.exists(FILE_SPELL_DAMAGE)) damage = file.get_int(FILE_SPELL_DAMAGE);
	if (file.exists(FILE_SPELL_COLOR)) {
		std::istringstream stream(file.get_string(FILE_SPELL_COLOR));
		int r, g, b, a;
		stream >> r >> g >> b >> a;
		color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
	}

	spell = new SpellComponent(entity, max_dis, speed, death_time, script_name, script, damage, color);
}

void load_magic(FileReader &file, Entity *entity, MagicComponent *&magic) {
	int main_spell_id = 0;
	int cast_speed = 0;

	if (file.exists(FILE_MAGIC_MAIN_SPELL_ID)) main_spell_id = file.get_int(FILE_MAGIC_MAIN_SPELL_ID);
	if (file.exists(FILE_MAGIC_CAST_SPEED)) cast_speed = file.get_int(FILE_MAGIC_CAST_SPEED);

	magic = new MagicComponent(entity, main_spell_id, cast_speed);
}

void load_player(FileReader &file, Entity *entity, PlayerComponent *&player) {
	std::string name = "";

	if (file.exists(FILE_PLAYER_NAME)) name = file.get_string(FILE_PLAYER_NAME);

	player = new PlayerComponent(entity, name);
}

void load_enemy(FileReader &file, Entity *entity, EnemyComponent *&enemy) {
	std::string name = " ";
	std::string script = " ";
	Combat_Range combat_range = { 0, 0 };

	if (file.exists(FILE_ENEMY_NAME)) name = file.get_string(FILE_ENEMY_NAME);
	if (file.exists(FILE_ENEMY_SCRIPT)) script = file.get_string(FILE_ENEMY_SCRIPT);
	if (file.exists(FILE_ENEMY_COMBAT_RANGEW)) combat_range.w = file.get_int(FILE_ENEMY_COMBAT_RANGEW);
	if (file.exists(FILE_ENEMY_COMBAT_RANGEH)) combat_range.h = file.get_int(FILE_ENEMY_COMBAT_RANGEH);

	enemy = new EnemyComponent(entity, name, script, combat_range);
}

void load_effect(FileReader &file, Entity *entity, EffectComponent *&effect) {
	std::string name = " ";
	std::string script = " ";
	int rand_rotation = 0;
	int rand_time = 0;

	if (file.exists(FILE_EFFECT_NAME)) name = file.get_string(FILE_EFFECT_NAME);
	if (file.exists(FILE_EFFECT_SCRIPT)) script = file.get_string(FILE_EFFECT_SCRIPT);
	if (file.exists(FILE_EFFECT_RAND_ROTATION)) rand_rotation = file.get_int(FILE_EFFECT_RAND_ROTATION);
	if (file.exists(FILE_EFFECT_RAND_TIME)) rand_time = file.get_int(FILE_EFFECT_RAND_TIME);

	effect = new EffectComponent(entity, name, script, rand_rotation, rand_time);
}

void load_combat(FileReader &file, Entity *entity, CombatComponent *&combat) {
	int max_health = 0, max_mana = 0;
	int damage = 0, armor = 0;
	int hps = 0, mps = 0;
	int combat_time = 10000;

	if (file.exists(FILE_COMBAT_MAX_HEALTH)) max_health = file.get_int(FILE_COMBAT_MAX_HEALTH);
	if (file.exists(FILE_COMBAT_MAX_MANA)) max_mana = file.get_int(FILE_COMBAT_MAX_MANA);
	if (file.exists(FILE_COMBAT_DAMAGE)) damage = file.get_int(FILE_COMBAT_DAMAGE);
	if (file.exists(FILE_COMBAT_ARMOR)) armor = file.get_int(FILE_COMBAT_ARMOR);
	if (file.exists(FILE_COMBAT_HPS)) hps = file.get_int(FILE_COMBAT_HPS);
	if (file.exists(FILE_COMBAT_MPS)) mps = file.get_int(FILE_COMBAT_MPS);
	if (file.exists(FILE_COMBAT_TIME)) combat_time = file.get_int(FILE_COMBAT_TIME);

	combat = new CombatComponent(entity, max_health, max_mana, damage, armor, hps, mps, combat_time);
}

void load_item(FileReader &file, Entity *entity, ItemComponent *&item) {
	int slot = 0;
	std::string name = "";
	int health = 0, mana = 0;
	int damage = 0, armor = 0;
	int hps = 0, mps = 0;
	bool is_equipable = false, is_useable = false;

	if (file.exists(FILE_ITEM_SLOT)) slot = file.get_int(FILE_ITEM_SLOT);
	if (file.exists(FILE_ITEM_NAME)) name = file.get_string(FILE_ITEM_NAME);
	if (file.exists(FILE_ITEM_HEALTH)) health = file.get_int(FILE_ITEM_HEALTH);
	if (file.exists(FILE_ITEM_MANA)) mana = file.get_int(FILE_ITEM_MANA);
	if (file.exists(FILE_ITEM_DAMAGE)) damage = file.get_int(FILE_ITEM_DAMAGE);
	if (file.exists(FILE_ITEM_ARMOR)) armor = file.get_int(FILE_ITEM_ARMOR);
	if (file.exists(FILE_ITEM_HPS)) hps = file.get_int(FILE_ITEM_HPS);
	if (file.exists(FILE_ITEM_MPS)) mps = file.get_int(FILE_ITEM_MPS);
	if (file.exists(FILE_ITEM_EQUIPABLE)) is_equipable = file.get_bool(FILE_ITEM_EQUIPABLE);
	if (file.exists(FILE_ITEM_USEABLE)) is_useable = file.get_bool(FILE_ITEM_USEABLE);

	item = new ItemComponent(entity, slot, name, health, mana, damage, armor, hps, mps, is_equipable, is_useable);
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

	std::string locate_file_path = ENTITY_FOLDER + STYPE(entity->get_type()) + "/" + STYPE(entity->get_type()) + ".txt";
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

	if (file.exists(FILE_TEXTURE_ID)) entity->set_texture_id(file.get_int(FILE_TEXTURE_ID));

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

	if (file.exists(FILE_EFFECT_COMPONENT)) {
		EffectComponent *effect = nullptr;
		load_effect(file, entity, effect);
		entity->add_component(effect);
		numComponents++;
	}

	if (file.exists(FILE_COMBAT_COMPONENT)) {
		CombatComponent *combat = nullptr;
		load_combat(file, entity, combat);
		entity->add_component(combat);
		numComponents++;
	}

	if (file.exists(FILE_ITEM_COMPONENT)) {
		ItemComponent *item = nullptr;
		load_item(file, entity, item);
		entity->add_component(item);
		numComponents++;
	}

	//Environment::get().get_log()->print("- " + std::to_string(numComponents) + " Component(s).", "\n", false);

	return true;
}