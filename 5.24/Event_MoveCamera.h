
#include "Event.h"
#include "Camera.h"

#ifndef EVENT_MOVE_CAMERA
#define EVENT_MOVE_CAMERA

struct Event_MoveCamera : public Event {
	Event_MoveCamera(Camera *camera, int direction) :
		m_camera		( camera ),
		m_direction		( direction )
	{}

	Camera *m_camera;
	int m_direction;
};

#endif