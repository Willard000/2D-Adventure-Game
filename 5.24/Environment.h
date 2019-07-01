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

	void set_mode(int mode);
	void set_clock(Clock *clock);
	void set_log(Log *log);
	void set_window(Window *window);
	void set_script_manager(ScriptManager *scriptManager);
	void set_resource_manager(ResourceManager *resourceManager);
	void set_input_manager(InputManager *inputManager);
	void set_ui_manager(UIManager *uiManager);

	int get_mode();
	Clock *get_clock();
	Log *get_log();
	Window *get_window();
	ScriptManager *get_script_manager();
	ResourceManager *get_resource_manager();
	InputManager *get_input_manager();
	UIManager *get_ui_manager();

	void shutdown();
	static Environment &get();
private:
	int _mode;
	Clock *_clock;
	Log *_log;
	Window *_window;
	ScriptManager *_script_manager;
	ResourceManager *_resource_manager;
	InputManager *_input_manager;
	UIManager *_ui_manager;

	static Environment *_instance;
};

#endif