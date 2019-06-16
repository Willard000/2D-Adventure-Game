#include <cassert>

#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class LogManager;
class WindowManager;
class ScriptManager;
class ResourceManager;
class InputManager;

class Environment {
public:
	Environment();
	~Environment();

	void setClock(Clock *clock);
	void setLogManager(LogManager *logManager);
	void setWindowManager(WindowManager *windowManager);
	void setScriptManager(ScriptManager *scriptManager);
	void setResourceManager(ResourceManager *resourceManager);
	void setInputManager(InputManager *inputManager);

	Clock *getClock();
	LogManager *getLogManager();
	WindowManager *getWindowManager();
	ScriptManager *getScriptManager();
	ResourceManager *getResourceManager();
	InputManager *getInputManager();

	void shutdown();
	static Environment &get();
private:
	Clock *_clock;
	LogManager *_logManager;
	WindowManager *_windowManager;
	ScriptManager *_scriptManager;
	ResourceManager *_resourceManager;
	InputManager *_inputManager;

	static Environment *_instance;
};

#endif