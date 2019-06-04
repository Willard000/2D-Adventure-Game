#ifndef HANDLER_FUNCTION_H
#define HANDLER_FUNCTION_H

struct Event;

class HandlerFunction {
public:
	void execute(Event *event) { call(event); }
private:
	virtual void call(Event *event) = 0;
};

#endif