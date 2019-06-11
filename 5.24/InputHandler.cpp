#include "InputHandler.h"

InputHandler::InputHandler() : EventHandler() {
	subscribe(this, &InputHandler::moveEntity);
	subscribe(this, &InputHandler::moveCamera);
}

void InputHandler::moveEntity(Event_MoveEntity *event) {
	MoveableComponent *moveable = GetMoveable(event->entity);
	if (moveable != nullptr) {
		moveable->move(event->direction, event->time);
	}
}

void InputHandler::moveCamera(Event_MoveCamera *event) {
	event->window->moveCamera(event->direction, event->time);
}