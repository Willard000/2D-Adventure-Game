#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class LogManager;
class WindowManager;
class ScriptManager;
class ResourceManager;
class InputManager;
class UIManager;

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
	void setUIManager(UIManager *uiManager);

	Clock *getClock();
	LogManager *getLogManager();
	WindowManager *getWindowManager();
	ScriptManager *getScriptManager();
	ResourceManager *getResourceManager();
	InputManager *getInputManager();
	UIManager *getUIManager();

	void shutdown();
	static Environment &get();
private:
	Clock *_clock;
	LogManager *_logManager;
	WindowManager *_windowManager;
	ScriptManager *_scriptManager;
	ResourceManager *_resourceManager;
	InputManager *_inputManager;
	UIManager *_uiManager;

	static Environment *_instance;
};

#endif