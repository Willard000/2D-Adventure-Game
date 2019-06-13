#include "InputHandler.h"

#include "MoveableComponent.h"

InputHandler::InputHandler() : EventHandler() {
	subscribe(this, &InputHandler::moveEntity);
	subscribe(this, &InputHandler::moveCamera);
}

void InputHandler::moveEntity(Event_MoveEntity *event) {
	MoveableComponent *moveable = GetMoveable(event->entity);
	if (moveable != nullptr) {
		moveable->move(event->direction);
	}
}

void InputHandler::moveCamera(Event_MoveCamera *event) {
	event->camera->move(event->direction);
}