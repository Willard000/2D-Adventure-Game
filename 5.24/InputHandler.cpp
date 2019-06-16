#include "InputHandler.h"

#include "PositionComponent.h"

InputHandler::InputHandler() : EventHandler() {
	subscribe(this, &InputHandler::moveEntity);
	subscribe(this, &InputHandler::moveCamera);
}

void InputHandler::moveEntity(Event_MoveEntity *event) {
	PositionComponent *position = GetPosition(event->entity);
	if (position) {
		position->move(event->direction);
	}
}

void InputHandler::moveCamera(Event_MoveCamera *event) {
	event->camera->move(event->direction);
}