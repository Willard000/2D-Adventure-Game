#include "ComponentLoader.h"

#include <iostream>
#include <string>

#include "FileReader.h"

#include "Component.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

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
	std::cout << "Loading Entity with type: " << entity->get_type() << " type_id: " << entity->get_type_id() << std::endl;
	std::string locate_file_path = ENTITY_BASE_PATH + entity->get_type() + "/" + entity->get_type() + ".txt";
	FileReader locate_file(locate_file_path.c_str());
	std::string entity_type_id = std::to_string(entity->get_type_id());

	if (!locate_file.exists(entity_type_id)) {
		std::cout << "No entity with type_id of: \"" << entity->get_type_id() << "\" exits." << std::endl;
		return;
	}

	FileReader file(locate_file.get_string(entity_type_id).c_str());

	std::cout << "Loaded: ";
	int numComponents = 0;

	if (file.exists(FILE_POSITION_COMPONENT)) {
		PositionComponent *position = nullptr;
		loadPosition(file, entity, position);
		entity->addComponent(position);
		numComponents++;
		std::cout << "Position ";
	}

	if (file.exists(FILE_SPRITE_COMPONENT)) {
		SpriteComponent *sprite = nullptr;
		loadSprite(file, entity, sprite);
		entity->addComponent(sprite);
		numComponents++;
		std::cout << "Sprite ";
	}

	std::cout << "- " << numComponents << " Component(s)." << std::endl;

	entity->is_loaded();
}