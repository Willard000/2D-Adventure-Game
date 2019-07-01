
#include "EventHandler.h"

#include "Event_MoveEntity.h"
#include "Event_MoveCamera.h"

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class InputHandler : public EventHandler {
public:
	InputHandler();
private:
	void move_entity(Event_MoveEntity *event);
	void move_camera(Event_MoveCamera *event);
};

#endif