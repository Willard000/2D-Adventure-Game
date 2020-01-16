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
#include "InteractComponent.h"
#include "NPCComponent.h"

#include "Environment.h"
#include "Log.h"

#include "Globals.h"

#define FILE_TEXTURE_ID "itexture_id"
#define FILE_ENTITY_NAME "sentity_name"

#define FILE_POSITION_COMPONENT "Position"
#define FILE_SPRITE_COMPONENT "Sprite"
#define FILE_SPELL_COMPONENT "Spell"
#define FILE_MAGIC_COMPONENT "Magic"
#define FILE_PLAYER_COMPONENT "Player"
#define FILE_ENEMY_COMPONENT "Enemy"
#define FILE_EFFECT_COMPONENT "Effect"
#define FILE_COMBAT_COMPONENT "Combat"
#define FILE_ITEM_COMPONENT "Item"
#define FILE_INTERACT_COMPONENT "Interact"
#define FILE_NPC_COMPONENT "NPC"

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
#define FILE_SPELL_MANA_COST "ispell_mana_cost"
#define FILE_SPELL_TYPE "ispell_type"
#define FILE_SPELL_COLOR "spell_color"

#define FILE_MAGIC_MAIN_SPELL_ID "imain_spell_id"
#define FILE_MAGIC_SECOND_SPELL_ID "isecond_spell_id"
#define FILE_MAGIC_CAST_SPEED "icast_speed"

#define FILE_PLAYER_NAME "splayer_name"

#define FILE_ENEMY_NAME "senemy_name"
#define FILE_ENEMY_SCRIPT "senemy_script"
#define FILE_ENEMY_COMBAT_RANGEW "ienemy_combat_range_w"
#define FILE_ENEMY_COMBAT_RANGEH "ienemy_combat_range_h"
#define FILE_ENEMY_DROP_TABLE "enemy_drop_table"

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
#define FILE_COMBAT_DRAIN "icombat_drain"
#define FILE_COMBAT_LUCK "icombat_luck"
#define FILE_COMBAT_EXP "icombat_exp"
#define FILE_COMBAT_TIME "icombat_time"

#define FILE_ITEM_SLOT "iitem_slot"
#define FILE_ITEM_NAME "sitem_name"
#define FILE_ITEM_INFO "sitem_info"
#define FILE_ITEM_HEALTH "iitem_health"
#define FILE_ITEM_MANA "iitem_mana"
#define FILE_ITEM_DAMAGE "iitem_damage"
#define FILE_ITEM_ARMOR "iitem_armor"
#define FILE_ITEM_HPS "iitem_hps"
#define FILE_ITEM_MPS "iitem_mps"
#define FILE_ITEM_DRAIN "iitem_drain"
#define FILE_ITEM_SPEED "iitem_speed"
#define FILE_ITEM_LUCK "iitem_luck"
#define FILE_ITEM_DURATION "iitem_duration"
#define FILE_ITEM_SELL_VALUE "iitem_sell_value"
#define FILE_ITEM_DROP_CHANCE "iitem_drop_chance"
#define FILE_ITEM_EQUIPABLE "bitem_equipable"
#define FILE_ITEM_USEABLE "bitem_useable"
#define FILE_ITEM_BUFFABLE "bitem_buffable"
#define FILE_ITEM_STACKABLE "bitem_stackable"
#define FILE_ITEM_SCRIPT_NAME "sitem_script_name"
#define FILE_ITEM_SCRIPT "sitem_script"
#define FILE_ITEM_COLOR "item_color"

#define FILE_INTERACT_NAME "sinteract_name"
#define FILE_INTERACT_SCRIPT_NAME "sinteract_script_name"
#define FILE_INTERACT_SCRIPT "sinteract_script"
#define FILE_INTERACT_UI_INFO "sinteract_ui_info"

#define FILE_NPC_SCRIPT_NAME "snpc_script_name"
#define FILE_NPC_SCRIPT "snpc_script"
#define FILE_NPC_QUEST_ID "inpc_quest_id"
#define FILE_NPC_HAS_SHOP "bnpc_has_shop"
#define FILE_NPC_SHOP "npc_shop"

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
	int mana_cost = 0;
	int spell_type = SPELL_TYPE::OTHER;
	SDL_Color color = { 0, 0, 0, 255 };

	if (file.exists(FILE_SPELL_MAX_DIS)) max_dis = file.get_float(FILE_SPELL_MAX_DIS);
	if (file.exists(FILE_SPELL_SPEED)) speed = file.get_float(FILE_SPELL_SPEED);
	if (file.exists(FILE_SPELL_DEATH_TIME)) death_time = file.get_int(FILE_SPELL_DEATH_TIME);
	if (file.exists(FILE_SPELL_SCRIPT_NAME)) script_name = file.get_string(FILE_SPELL_SCRIPT_NAME);
	if (file.exists(FILE_SPELL_SCRIPT)) script = file.get_string(FILE_SPELL_SCRIPT);
	if (file.exists(FILE_SPELL_DAMAGE)) damage = file.get_int(FILE_SPELL_DAMAGE);
	if (file.exists(FILE_SPELL_MANA_COST)) mana_cost = file.get_int(FILE_SPELL_MANA_COST);
	if (file.exists(FILE_SPELL_TYPE)) spell_type = file.get_int(FILE_SPELL_TYPE);
	if (file.exists(FILE_SPELL_COLOR)) {
		std::istringstream stream(file.get_string(FILE_SPELL_COLOR));
		int r, g, b, a;
		stream >> r >> g >> b >> a;
		color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
	}

	spell = new SpellComponent(entity, max_dis, speed, death_time, script_name, script, damage, mana_cost, spell_type, color);
}

void load_magic(FileReader &file, Entity *entity, MagicComponent *&magic) {
	int main_spell_id = 0;
	int second_spell_id = 0;
	int cast_speed = 0;

	if (file.exists(FILE_MAGIC_MAIN_SPELL_ID)) main_spell_id = file.get_int(FILE_MAGIC_MAIN_SPELL_ID);
	if (file.exists(FILE_MAGIC_SECOND_SPELL_ID)) second_spell_id = file.get_int(FILE_MAGIC_SECOND_SPELL_ID);
	if (file.exists(FILE_MAGIC_CAST_SPEED)) cast_speed = file.get_int(FILE_MAGIC_CAST_SPEED);

	magic = new MagicComponent(entity, main_spell_id, second_spell_id, cast_speed);
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
	std::vector<Drop> drop_table;

	if (file.exists(FILE_ENEMY_NAME)) name = file.get_string(FILE_ENEMY_NAME);
	if (file.exists(FILE_ENEMY_SCRIPT)) script = file.get_string(FILE_ENEMY_SCRIPT);
	if (file.exists(FILE_ENEMY_COMBAT_RANGEW)) combat_range.w = file.get_int(FILE_ENEMY_COMBAT_RANGEW);
	if (file.exists(FILE_ENEMY_COMBAT_RANGEH)) combat_range.h = file.get_int(FILE_ENEMY_COMBAT_RANGEH);
	if (file.exists(FILE_ENEMY_DROP_TABLE)) {
		std::istringstream table(file.get_string(FILE_ENEMY_DROP_TABLE));
		int id, drop_chance;
		while (table >> id >> drop_chance) {
			drop_table.push_back({ id, drop_chance });
		}
	}

	enemy = new EnemyComponent(entity, name, script, combat_range, drop_table);
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
	int drain = 0;
	int luck = 0;
	int exp = 0;
	int combat_time = 10000;

	if (file.exists(FILE_COMBAT_MAX_HEALTH)) max_health = file.get_int(FILE_COMBAT_MAX_HEALTH);
	if (file.exists(FILE_COMBAT_MAX_MANA)) max_mana = file.get_int(FILE_COMBAT_MAX_MANA);
	if (file.exists(FILE_COMBAT_DAMAGE)) damage = file.get_int(FILE_COMBAT_DAMAGE);
	if (file.exists(FILE_COMBAT_ARMOR)) armor = file.get_int(FILE_COMBAT_ARMOR);
	if (file.exists(FILE_COMBAT_HPS)) hps = file.get_int(FILE_COMBAT_HPS);
	if (file.exists(FILE_COMBAT_MPS)) mps = file.get_int(FILE_COMBAT_MPS);
	if (file.exists(FILE_COMBAT_DRAIN)) drain = file.get_int(FILE_COMBAT_DRAIN);
	if (file.exists(FILE_COMBAT_LUCK)) luck = file.get_int(FILE_COMBAT_LUCK);
	if (file.exists(FILE_COMBAT_EXP)) exp = file.get_int(FILE_COMBAT_EXP);
	if (file.exists(FILE_COMBAT_TIME)) combat_time = file.get_int(FILE_COMBAT_TIME);

	combat = new CombatComponent(entity, max_health, max_mana, damage, armor, hps, mps, drain, luck, exp, combat_time);
}

void load_item(FileReader &file, Entity *entity, ItemComponent *&item) {
	int slot = 0;
	std::string name, info;
	int health = 0, mana = 0;
	int damage = 0, armor = 0;
	int hps = 0, mps = 0;
	int leech = 0, drain = 0;
	int speed = 0, luck = 0;
	int sell_value = 0;
	int duration = 0;
	bool is_equipable = false, is_useable = false, is_buffable = false, is_stackable = false;
	std::string script, script_name;
	SDL_Color color = { 255, 255, 255, 255 };

	if (file.exists(FILE_ITEM_SLOT)) slot = file.get_int(FILE_ITEM_SLOT);
	if (file.exists(FILE_ITEM_NAME)) name = file.get_string(FILE_ITEM_NAME);
	if (file.exists(FILE_ITEM_INFO))  info = file.get_string(FILE_ITEM_INFO);	
	if (file.exists(FILE_ITEM_HEALTH)) health = file.get_int(FILE_ITEM_HEALTH);
	if (file.exists(FILE_ITEM_MANA)) mana = file.get_int(FILE_ITEM_MANA);
	if (file.exists(FILE_ITEM_DAMAGE)) damage = file.get_int(FILE_ITEM_DAMAGE);
	if (file.exists(FILE_ITEM_ARMOR)) armor = file.get_int(FILE_ITEM_ARMOR);
	if (file.exists(FILE_ITEM_HPS)) hps = file.get_int(FILE_ITEM_HPS);
	if (file.exists(FILE_ITEM_MPS)) mps = file.get_int(FILE_ITEM_MPS);
	if (file.exists(FILE_ITEM_DRAIN)) drain = file.get_int(FILE_ITEM_DRAIN);
	if (file.exists(FILE_ITEM_SPEED)) speed = file.get_int(FILE_ITEM_SPEED);
	if (file.exists(FILE_ITEM_LUCK)) luck = file.get_int(FILE_ITEM_LUCK);
	if (file.exists(FILE_ITEM_SELL_VALUE)) sell_value = file.get_int(FILE_ITEM_SELL_VALUE);
	if (file.exists(FILE_ITEM_DURATION)) duration = file.get_int(FILE_ITEM_DURATION);
	if (file.exists(FILE_ITEM_EQUIPABLE)) is_equipable = file.get_bool(FILE_ITEM_EQUIPABLE);
	if (file.exists(FILE_ITEM_USEABLE)) is_useable = file.get_bool(FILE_ITEM_USEABLE);
	if (file.exists(FILE_ITEM_BUFFABLE)) is_buffable = file.get_bool(FILE_ITEM_BUFFABLE);
	if (file.exists(FILE_ITEM_STACKABLE)) is_stackable = file.get_bool(FILE_ITEM_STACKABLE);
	if (file.exists(FILE_ITEM_SCRIPT_NAME)) script_name = file.get_string(FILE_ITEM_SCRIPT_NAME);
	if (file.exists(FILE_ITEM_SCRIPT)) script = file.get_string(FILE_ITEM_SCRIPT);
	if (file.exists(FILE_ITEM_COLOR)) {
		std::istringstream stream(file.get_string(FILE_ITEM_COLOR));
		int r, g, b, a;
		stream >> r >> g >> b >> a;
		color = { (Uint8)r, (Uint8)g, (Uint8)b, (Uint8)a };
	}

	item = new ItemComponent(entity, slot, name, info, health, mana, damage, armor, hps, mps, drain, speed, luck, duration, sell_value, sell_value * 2,
		is_equipable, is_useable, is_buffable, is_stackable, script_name, script, color);
}

void load_interact(FileReader &file, Entity *entity, InteractComponent *&interact) {
	std::string name = "";
	std::string script_name = "";
	std::string script = "";
	std::string ui_info = "";

	if (file.exists(FILE_INTERACT_NAME)) name = file.get_string(FILE_INTERACT_NAME);
	if (file.exists(FILE_INTERACT_SCRIPT_NAME)) script_name = file.get_string(FILE_INTERACT_SCRIPT_NAME);
	if (file.exists(FILE_INTERACT_SCRIPT)) script = file.get_string(FILE_INTERACT_SCRIPT);
	if (file.exists(FILE_INTERACT_UI_INFO)) ui_info = file.get_string(FILE_INTERACT_UI_INFO);

	interact = new InteractComponent(entity, name, script_name, script, ui_info);
}

void load_npc(FileReader &file, Entity *entity, NPCComponent *&npc) {
	std::string script_name = "";
	std::string script = "";
	int quest_id = 0;
	bool has_shop = false;
	std::vector<int> item_ids;

	if (file.exists(FILE_NPC_SCRIPT_NAME)) script_name = file.get_string(FILE_NPC_SCRIPT_NAME);
	if (file.exists(FILE_NPC_SCRIPT)) script = file.get_string(FILE_NPC_SCRIPT);
	if (file.exists(FILE_NPC_QUEST_ID)) quest_id = file.get_int(FILE_NPC_QUEST_ID);
	if (file.exists(FILE_NPC_HAS_SHOP)) has_shop = file.get_int(FILE_NPC_HAS_SHOP);
	if (file.exists(FILE_NPC_SHOP)) {
		std::istringstream stream(file.get_string(FILE_NPC_SHOP));
		int id; while (stream >> id) item_ids.push_back(id);
	}

	npc = new NPCComponent(entity, script_name, script, quest_id, has_shop, item_ids);
}

bool load_components(Entity *entity) {
	/*
	Environment::get().get_log()->print(
		"Loading Entity with type: \n "e
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
	if (file.exists(FILE_ENTITY_NAME)) entity->set_name(file.get_string(FILE_ENTITY_NAME));

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

	if (file.exists(FILE_INTERACT_COMPONENT)) {
		InteractComponent *interact = nullptr;
		load_interact(file, entity, interact);
		entity->add_component(interact);
		numComponents++;
	}

	if (file.exists(FILE_NPC_COMPONENT)) {
		NPCComponent *npc = nullptr;
		load_npc(file, entity, npc);
		entity->add_component(npc);
		numComponents++;
	}

	//Environment::get().get_log()->print("- " + std::to_string(numComponents) + " Component(s).", "\n", false);

	return true;
}