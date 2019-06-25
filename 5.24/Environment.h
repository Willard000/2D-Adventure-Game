#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class Log;
class Window;
class ScriptManager;
class ResourceManager;
class InputManager;
class UIManager;

enum {
	MODE_GAME = 1,
	MODE_EDITOR
};

class Environment {
public:
	Environment();
	~Environment();

	void setMode(int mode);
	void setClock(Clock *clock);
	void setLog(Log *log);
	void setWindow(Window *window);
	void setScriptManager(ScriptManager *scriptManager);
	void setResourceManager(ResourceManager *resourceManager);
	void setInputManager(InputManager *inputManager);
	void setUIManager(UIManager *uiManager);

	int getMode();
	Clock *getClock();
	Log *getLog();
	Window *getWindow();
	ScriptManager *getScriptManager();
	ResourceManager *getResourceManager();
	InputManager *getInputManager();
	UIManager *getUIManager();

	void shutdown();
	static Environment &get();
private:
	int _mode;
	Clock *_clock;
	Log *_log;
	Window *_window;
	ScriptManager *_scriptManager;
	ResourceManager *_resourceManager;
	InputManager *_inputManager;
	UIManager *_uiManager;

	static Environment *_instance;
};

#endif