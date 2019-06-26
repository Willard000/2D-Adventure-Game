#include "ComponentLoader.h"

#include <iostream>
#include <string>

#include "FileReader.h"

#include "Component.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#include "Environment.h"
#include "Log.h"

#define FILE_POSITION_COMPONENT "Position"
#define FILE_SPRITE_COMPONENT "Sprite"

#define FILE_POSITION_WIDTH "iwidth"
#define FILE_POSITION_HEIGHT "iheight"
#define FILE_POSITION_SPEED "dspeed"

#define FILE_SPRITE_WIDTH "iwidth"
#define FILE_SPRITE_HEIGHT "iheight"
#define FILE_SPRITE_TIME "sprite_itime"

const char *ENTITY_BASE_PATH = "Data/Entities/";

void loadPosition(FileReader &file, Entity *entity, PositionComponent *&position) {
	int w = 32, h = 32;
	double speed = 0.0;

	if (file.exists(FILE_POSITION_WIDTH)) { w = file.get_int(FILE_POSITION_WIDTH); }
	if (file.exists(FILE_POSITION_HEIGHT)) { h = file.get_int(FILE_POSITION_HEIGHT); }
	if (file.exists(FILE_POSITION_SPEED)) { speed = file.get_double(FILE_POSITION_SPEED); }

	position = new PositionComponent(entity, 0.0, 0.0, w, h, speed);
}

void loadSprite(FileReader &file, Entity *entity, SpriteComponent *&sprite) {
	int w = 0, h = 0, time = 0;

	if (file.exists(FILE_SPRITE_WIDTH)) { w = file.get_int(FILE_SPRITE_WIDTH); }
	if (file.exists(FILE_SPRITE_HEIGHT)) { h = file.get_int(FILE_SPRITE_HEIGHT); }
	if (file.exists(FILE_SPRITE_TIME)) { time = file.get_int(FILE_SPRITE_TIME); }

	sprite = new SpriteComponent(entity, w, h, time);
}

void loadComponents(Entity *entity) {
	Environment::get().getLog()->print(
		"Loading Entity with type: \n "
		+ entity->get_type()
		+ "\n type_id: "
		+ std::to_string(entity->get_type_id())
	);

	std::string locate_file_path = ENTITY_BASE_PATH + entity->get_type() + "/" + entity->get_type() + ".txt";
	FileReader locate_file(locate_file_path.c_str());
	std::string entity_type_id = std::to_string(entity->get_type_id());

	if (!locate_file.exists(entity_type_id)) {
		Environment::get().getLog()->print(
			"No entity with type_id of: \""
			+ std::to_string(entity->get_type_id())
			+ "\" exits."
		);
		return;
	}

	FileReader file(locate_file.get_string(entity_type_id).c_str());

	int numComponents = 0;

	Environment::get().getLog()->print("Loading Components: ", "\n", false);

	if (file.exists(FILE_POSITION_COMPONENT)) {
		PositionComponent *position = nullptr;
		loadPosition(file, entity, position);
		entity->addComponent(position);
		numComponents++;
		Environment::get().getLog()->print("Position ", "", false);
	}

	if (file.exists(FILE_SPRITE_COMPONENT)) {
		SpriteComponent *sprite = nullptr;
		loadSprite(file, entity, sprite);
		entity->addComponent(sprite);
		numComponents++;
		Environment::get().getLog()->print("Sprite ", "", false);
	}

	Environment::get().getLog()->print("- " + std::to_string(numComponents) + " Component(s).", "\n", false);

	entity->is_loaded();
}