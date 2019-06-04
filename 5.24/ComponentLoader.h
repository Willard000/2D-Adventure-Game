#include <iostream>
#include <string>

#include "FileReader.h"

#include "Entity.h"

#include "Component.h"
#include "PositionComponent.h"
#include "MoveableComponent.h"
#include "SpriteComponent.h"

#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

#define FILE_POSITION_COMPONENT "Position"
#define FILE_MOVEABLE_COMPONENT "Moveable"
#define FILE_SPRITE_COMPONENT "Sprite"

#define POSITION_WIDTH "iwidth"
#define POSITION_HEIGHT "iheight"

#define MOVEABLE_WIDTH "iwidth"
#define MOVEABLE_HEIGHT "iheight"
#define MOVEABLE_SPEED "moveable_dspeed"

#define SPRITE_WIDTH "iwidth"
#define SPRITE_HEIGHT "iheight"
#define SPRITE_TIME "sprite_itime"


extern const char *ENTITY_BASE_PATH;

class ComponentLoader {
public:
	static void loadComponents(Entity *entity);

	static void loadPosition(FileReader &file, Entity *entity, PositionComponent *&position);
	static void loadMovable(FileReader &file, Entity *entity, MoveableComponent *&moveable);
	static void loadSprite(FileReader &file, Entity *entity, SpriteComponent *&sprite);
private:
};

#endif