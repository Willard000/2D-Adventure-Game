#include "HandlerFunction.h"

#ifndef MEMBER_HANDLER_FUNCTION_H
#define MEMBER_HANDLER_FUNCTION_H

struct Event;

template<class T, class EventType>
class MemberHandlerFunction : public HandlerFunction {
public:
	typedef void (T::*MemberFunction)(EventType *);

	MemberHandlerFunction(T *instance, MemberFunction memberFunction) :
		_instance(instance), _memberFunction(memberFunction) {};

	// call function of instance class 
	void call(Event *event) { (_instance->*_memberFunction)(static_cast<EventType *>(event)); }

	T *getInstance() { return _instance; }
	MemberFunction getMemberFunction() { return _memberFunction; }
private:
	T *_instance;
	MemberFunction _memberFunction;
};

#endif