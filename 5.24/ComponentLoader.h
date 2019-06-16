#include <iostream>
#include <string>

#include "FileReader.h"

#include "Entity.h"

#include "Component.h"
#include "PositionComponent.h"
#include "SpriteComponent.h"

#ifndef COMPONENT_LOADER_H
#define COMPONENT_LOADER_H

#define FILE_POSITION_COMPONENT "Position"
#define FILE_SPRITE_COMPONENT "Sprite"

#define FILE_POSITION_WIDTH "iwidth"
#define FILE_POSITION_HEIGHT "iheight"
#define FILE_POSITION_SPEED "dspeed"

#define FILE_SPRITE_WIDTH "iwidth"
#define FILE_SPRITE_HEIGHT "iheight"
#define FILE_SPRITE_TIME "sprite_itime"


extern const char *ENTITY_BASE_PATH;

class ComponentLoader {
public:
	static void loadComponents(Entity *entity);

	static void loadPosition(FileReader &file, Entity *entity, PositionComponent *&position);
	static void loadSprite(FileReader &file, Entity *entity, SpriteComponent *&sprite);
private:
};

#endif