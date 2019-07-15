#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

class Clock;
class Log;
class Window;
class Lua;
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
	void set_lua(Lua *lua);
	void set_resource_manager(ResourceManager *resource_manager);
	void set_input_manager(InputManager *input_manager);
	void set_ui_manager(UIManager *ui_manager);

	int get_mode();
	Clock *get_clock();
	Log *get_log();
	Window *get_window();
	Lua *get_lua();
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
	Lua *_lua;
	ResourceManager *_resource_manager;
	InputManager *_input_manager;
	UIManager *_ui_manager;

	static Environment *_instance;
};

#endif