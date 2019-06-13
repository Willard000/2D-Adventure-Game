
#include "Event.h"
#include "Camera.h"

#ifndef EVENT_MOVE_CAMERA
#define EVENT_MOVE_CAMERA

struct Event_MoveCamera : public Event {
	Event_MoveCamera(Camera *camera_, int direction_) {
		camera = camera_;
		direction = direction_;
	}

	Camera *camera;
	int direction;
};

#endif