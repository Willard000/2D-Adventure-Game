#include "Engine.h"

#include "Environment.h"
#include "Clock.h"
#include "Log.h"
#include "Window.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

#include "WindowLoader.h"

Engine::Engine() :
	_isRunning		( true )
{
	build_environment();

	_environment.get().get_resource_manager()->load_map(1);
}

void Engine::run() {
	_environment.get_log()->print("Engine Main Loop\n");

	while (_isRunning) {
		_environment.get_window()->get_renderer()->clear();

		_isRunning = _environment.get_input_manager()->get();

		_environment.get_input_manager()->update();

		_environment.get_script_manager()->run("Data/Lua/test.lua");

		_environment.get_resource_manager()->update();

		_environment.get_resource_manager()->render();

		_environment.get_window()->get_renderer()->render();

		if (_environment.get_clock()->update()) {
			std::string title = "Engine      Map: " + std::to_string(_environment.get_resource_manager()->get_map()->get_id()) + "     " +
				_environment.get_clock()->get_display_time() + "    " + std::to_string(_environment.get_clock()->get_fms());
			_environment.get_window()->set_title(title);
		}
	}

	_environment.get_log()->print("Shutting Down Engine\n");
	_environment.shutdown();
}

void Engine::build_environment() {
	_environment.set_mode(MODE_GAME);

	Clock *clock = new Clock();
	_environment.set_clock(clock);

	Log *log = new Log();
	_environment.set_log(log);

	Window *window = load_window();
	_environment.set_window(window);
	
	ScriptManager *script_manager = new ScriptManager();
	_environment.set_script_manager(script_manager);

	ResourceManager *resource_manager = new ResourceManager();
	_environment.set_resource_manager(resource_manager);

	InputManager *input_manager = new InputManager();
	_environment.set_input_manager(input_manager);
}