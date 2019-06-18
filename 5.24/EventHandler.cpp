#include "EventHandler.h"

EventHandler::~EventHandler() {
	for (auto it = _subscribers.begin(); it != _subscribers.end(); it++) {
		for (auto itt = it->second.begin(); itt != it->second.end(); itt++) {
			delete *itt;
		}
	}
}