#include "ComponentLoader.h"

const char *ENTITY_BASE_PATH = "Data/Entities/";

void ComponentLoader::loadComponents(Entity *entity) {
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
		ComponentLoader::loadPosition(file, entity, position);
		entity->addComponent(position);
		numComponents++;
		std::cout << "Position ";
	}

	if (file.exists(FILE_MOVEABLE_COMPONENT)) {
		MoveableComponent *moveable = nullptr;
		ComponentLoader::loadMovable(file, entity, moveable);
		entity->addComponent(moveable);
		numComponents++;
		std::cout << "Moveable ";
	}

	if (file.exists(FILE_SPRITE_COMPONENT)) {
		SpriteComponent *sprite = nullptr;
		ComponentLoader::loadSprite(file, entity, sprite);
		entity->addComponent(sprite);
		numComponents++;
		std::cout << "Sprite ";
	}

	std::cout << "- " << numComponents << " Component(s)." << std::endl;

	entity->is_loaded();
}

void ComponentLoader::loadPosition(FileReader &file, Entity *entity, PositionComponent *&position) {
	int w = 0, h = 0;

	if (file.exists(FILE_POSITION_WIDTH))		 { w = file.get_int(FILE_POSITION_WIDTH); }
	if (file.exists(FILE_POSITION_HEIGHT))		 { h = file.get_int(FILE_POSITION_HEIGHT); }

	position = new PositionComponent(entity, 0.0, 0.0, w, h);
}

void ComponentLoader::loadMovable(FileReader &file, Entity *entity, MoveableComponent *&moveable) {
	double speed = 0.0;
	int w = 0, h = 0;

	if (file.exists(FILE_MOVEABLE_WIDTH))		 { w = file.get_int(FILE_MOVEABLE_WIDTH); }
	if (file.exists(FILE_MOVEABLE_HEIGHT))		 { h = file.get_int(FILE_MOVEABLE_HEIGHT); }
	if (file.exists(FILE_MOVEABLE_SPEED))		 { speed = file.get_double(FILE_MOVEABLE_SPEED); }

	moveable = new MoveableComponent(entity, 0.0, 0.0, w, h, speed);
}

void ComponentLoader::loadSprite(FileReader &file, Entity *entity, SpriteComponent *&sprite) {
	int w = 0, h = 0, time = 0;

	if (file.exists(FILE_SPRITE_WIDTH))		    {		w = file.get_int(FILE_SPRITE_WIDTH);	}
	if (file.exists(FILE_SPRITE_HEIGHT))		{		h = file.get_int(FILE_SPRITE_HEIGHT);	}
	if (file.exists(FILE_SPRITE_TIME))			{		time = file.get_int(FILE_SPRITE_TIME);	}

	sprite = new SpriteComponent(entity, w, h, time);
}