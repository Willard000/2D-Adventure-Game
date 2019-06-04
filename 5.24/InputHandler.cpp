#include "InputHandler.h"

InputHandler::InputHandler() : EventHandler() {
	subscribe(this, &InputHandler::move);
}

void InputHandler::move(MoveEvent *move_event) {
	MoveableComponent *moveable = GetMoveable(move_event->entity);
	if (moveable != nullptr) {
		moveable->move(move_event->direction, move_event->time);
	}
}