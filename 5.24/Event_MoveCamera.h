
#include "Event.h"
#include "Window.h"

#ifndef EVENT_MOVE_CAMERA
#define EVENT_MOVE_CAMERA

struct Event_MoveCamera : public Event {
	Event_MoveCamera(Window *window_, int direction_, double time_) {
		window = window_;
		direction = direction_;
		time = time_;
	}

	Window *window;
	int direction;
	double time;
};

#endif