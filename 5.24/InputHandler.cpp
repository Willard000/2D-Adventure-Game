#include "InputHandler.h"

#include "PositionComponent.h"

InputHandler::InputHandler() :
	EventHandler() 
{
	subscribe(this, &InputHandler::move_entity);
	subscribe(this, &InputHandler::move_camera);
}

void InputHandler::move_entity(Event_MoveEntity *event) {
	PositionComponent *position = GetPosition(event->entity);
	if (position) {
		position->move(event->direction);
	}
}

void InputHandler::move_camera(Event_MoveCamera *event) {
	event->camera->move(event->direction);
}