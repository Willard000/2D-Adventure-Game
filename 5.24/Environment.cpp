#include "Environment.h"

#include <cassert>

#include "Clock.h"
#include "Log.h"
#include "Window.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "UIManager.h"

Environment *Environment::_instance = nullptr;

Environment::Environment() :
	_mode					( NULL ),
	_clock					( nullptr ),
	_log					( nullptr ),
	_window					( nullptr ),
	_scriptManager			( nullptr ),
	_resourceManager		( nullptr ),
	_inputManager			( nullptr ),
	_uiManager				( nullptr )
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {
	shutdown();
	_instance = nullptr;
}

void Environment::shutdown() {
	delete _uiManager;
	_uiManager = nullptr;

	delete _inputManager;
	_inputManager = nullptr;

	delete _resourceManager;
	_resourceManager = nullptr;

	delete _scriptManager;
	_scriptManager = nullptr;

	delete _window;
	_window = nullptr;

	delete _log;
	_log = nullptr;

	delete _clock;
	_clock = nullptr;
}

Environment &Environment::get(){
	assert(_instance);
	return *_instance;
}

void Environment::setMode(int mode) {
	_mode = mode;
}

void Environment::setClock(Clock *clock) {
	_clock = clock;
}

void Environment::setLog(Log *log) {
	_log = log;
}

void Environment::setWindow(Window *window) {
	_window = window;
}

void Environment::setScriptManager(ScriptManager *scriptManager) {
	_scriptManager = scriptManager;
}

void Environment::setResourceManager(ResourceManager *resourceManager) {
	_resourceManager = resourceManager;
}

void Environment::setInputManager(InputManager *inputManager) {
	_inputManager = inputManager;
}

void Environment::setUIManager(UIManager *uiManager) {
	_uiManager = uiManager;
}

int Environment::getMode() {
	return _mode;
}

Clock *Environment::getClock() {
	return _clock;
}

Log *Environment::getLog() {
	return _log;
}

Window *Environment::getWindow() {
	return _window;
}

ScriptManager *Environment::getScriptManager() {
	return _scriptManager;
}

ResourceManager *Environment::getResourceManager() {
	return _resourceManager;
}

InputManager *Environment::getInputManager() {
	return _inputManager;
}

UIManager *Environment::getUIManager() {
	return _uiManager;
}