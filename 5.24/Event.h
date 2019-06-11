#ifndef EVENT_H
#define EVENT_H

struct Event {

	enum Move {
		UP = 1,
		DOWN,
		LEFT,
		RIGHT,
		UPLEFT,
		UPRIGHT,
		DOWNLEFT,
		DOWNRIGHT
	};
};

#endif