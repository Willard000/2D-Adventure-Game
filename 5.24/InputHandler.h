
#include "EventHandler.h"
#include "MoveEvent.h"
#include "MoveableComponent.h"

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

class InputHandler : public EventHandler {
public:
	InputHandler();
private:
	void move(MoveEvent *move_event);
};

#endif