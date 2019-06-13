#include <cassert>

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class WindowManager;
class ResourceManager;
class ScriptManager;
class InputManager;

class Environment {
public:
	Environment();
	~Environment();

	void setClock(Clock *clock);
	void setWindowManager(WindowManager *windowManager);
	void setScriptManager(ScriptManager *scriptManager);
	void setResourceManager(ResourceManager *resourceManager);
	void setInputManager(InputManager *inputManager);

	Clock *getClock();
	WindowManager *getWindowManager();
	ScriptManager *getScriptManager();
	ResourceManager *getResourceManager();
	InputManager *getInputManager();

	static Environment &get();
private:
	Clock *_clock;
	WindowManager *_windowManager;
	ResourceManager *_resourceManager;
	ScriptManager *_scriptManager;
	InputManager *_inputManager;

	static Environment *_instance;
};

#endif