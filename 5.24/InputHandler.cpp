#include "InputHandler.h"

#include "PositionComponent.h"

InputHandler::InputHandler() :
	EventHandler() 
{
	subscribe(this, &InputHandler::moveEntity);
	subscribe(this, &InputHandler::moveCamera);
}

void InputHandler::moveEntity(Event_MoveEntity *event) {
	PositionComponent *position = GetPosition(event->m_entity);
	if (position) {
		position->move(event->m_direction);
	}
}

void InputHandler::moveCamera(Event_MoveCamera *event) {
	event->m_camera->move(event->m_direction);
}