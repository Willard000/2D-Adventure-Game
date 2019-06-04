#include "ComponentLoader.h"

const char *ENTITY_BASE_PATH = "Data/Entities/";

/*
void ComponentLoader::loadComponents(Entity *entity) {
	std::cout << "Loading Entity with type: " << entity->get_type() << " type_id: " << entity->get_type_id() << std::endl;
	std::string locate_file_path = ENTITY_BASE_PATH + entity->get_type() + "/" + entity->get_type() + ".txt";
	FileReader locate_file(locate_file_path.c_str());
	unsigned int locate_line = 0;
	bool found = false;
	while (locate_line < locate_file.size()) {
		if (locate_file.first_int(locate_line) == entity->get_type_id()) {
			found = true;
			break;
		}
		locate_line++;
	}

	if (!found) {
		std::cout << "No entity with type_id of: \"" << entity->get_type_id() << "\" exits." << std::endl;
		return;
	}

	FileReader file(locate_file.second_string(locate_line).c_str());

	std::cout << "Loaded: ";
	int numComponents = 0;
	for (unsigned int i = 0; i < file.size(); i++) {
		std::string data = file.first_string(i);

		if (data == FILE_POSITION_COMPONENT) {
			PositionComponent *position = nullptr;
			ComponentLoader::loadPosition(file, ++i, entity, position);
			entity->addComponent(position);
			numComponents++;
			std::cout << "Position ";
		}
		else if (data == FILE_MOVEABLE_COMPONENT) {
			MoveableComponent *moveable = nullptr;
			ComponentLoader::loadMovable(file, ++i, entity, moveable);
			entity->addComponent(moveable);
			numComponents++;
			std::cout << "Moveable ";
		}
		else if (data == FILE_SPRITE_COMPONENT) {
			SpriteComponent *sprite = nullptr;
			ComponentLoader::loadSprite(file, ++i, entity, sprite);
			entity->addComponent(sprite);
			numComponents++;
			std::cout << "Sprite ";
		}
	}
	std::cout << "- " << numComponents << " Component(s)." << std::endl;

	entity->is_loaded();
}

void ComponentLoader::loadPosition(FileReader &file, unsigned int &line, Entity *entity, PositionComponent *&position) {
	double x = 0, y = 0;
	int w = 0, h = 0;

	while (line < file.size()) {
		std::string data = file.first_string(line);

		if (data == POSITION_WIDTH) w = file.second_int(line);
		else if (data == POSITION_HEIGHT) h = file.second_int(line);
		else {
			if (line != file.size() - 1) {
				line--;
			}
			break;
		}

		line++;
	}

	position = new PositionComponent(entity, x, y, w, h);
}

void ComponentLoader::loadMovable(FileReader &file, unsigned int &line, Entity *entity, MoveableComponent *&moveable) {
	double x = 0, y = 0, speed = 0;
	int w = 0, h = 0;

	while(line < file.size()) {
		std::string data = file.first_string(line);

		if (data == MOVEABLE_WIDTH) w = file.second_int(line);
		else if (data == MOVEABLE_HEIGHT) h = file.second_int(line);
		else if (data == MOVEABLE_SPEED) speed = file.second_double(line);
		else {
			if(line != file.size() - 1) {
			line--;
			}
			break;
		}

		line++;
	}

	moveable = new MoveableComponent(entity, x, y, w, h, speed);
}

void ComponentLoader::loadSprite(FileReader &file, unsigned int &line, Entity *entity, SpriteComponent *&sprite) {
	int w, h, time;

	while (line < file.size()) {
		std::string data = file.first_string(line);

		if (data == SPRITE_WIDTH) w = file.second_int(line);
		else if (data == SPRITE_HEIGHT) h = file.second_int(line);
		else if (data == SPRITE_TIME) time = file.second_int(line);
		else {
			if (line != file.size() - 1) {
				line--;
			}
			break;
		}

		line++;
	}

	sprite = new SpriteComponent(entity, w, h, time);
}

*/

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

	if (file.exists(POSITION_WIDTH))		 { w = file.get_int(POSITION_WIDTH); }
	if (file.exists(POSITION_HEIGHT))		 { h = file.get_int(POSITION_HEIGHT); }

	position = new PositionComponent(entity, 0.0, 0.0, w, h);
}

void ComponentLoader::loadMovable(FileReader &file, Entity *entity, MoveableComponent *&moveable) {
	double speed = 0.0;
	int w = 0, h = 0;

	if (file.exists(MOVEABLE_WIDTH))		 { w = file.get_int(MOVEABLE_WIDTH); }
	if (file.exists(MOVEABLE_HEIGHT))		 { h = file.get_int(MOVEABLE_HEIGHT); }
	if (file.exists(MOVEABLE_SPEED))		 { speed = file.get_double(MOVEABLE_SPEED); }

	moveable = new MoveableComponent(entity, 0.0, 0.0, w, h, speed);
}

void ComponentLoader::loadSprite(FileReader &file, Entity *entity, SpriteComponent *&sprite) {
	int w = 0, h = 0, time = 0;

	if (file.exists(SPRITE_WIDTH))		    {		w = file.get_int(SPRITE_WIDTH);		}
	if (file.exists(SPRITE_HEIGHT))			{		h = file.get_int(SPRITE_HEIGHT);	}
	if (file.exists(SPRITE_TIME))			{		time = file.get_int(SPRITE_TIME);	}

	sprite = new SpriteComponent(entity, w, h, time);
}