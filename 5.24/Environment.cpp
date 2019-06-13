#include "Environment.h"

Environment *Environment::_instance = nullptr;

Environment::Environment()
	: _clock(0),
	_windowManager(0),
	_resourceManager(0),
	_scriptManager(0),
	_inputManager(0)
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {

}

Environment &Environment::get(){
	assert(_instance);
	return *_instance;
}

void Environment::setClock(Clock *clock) {
	_clock = clock;
}

void Environment::setWindowManager(WindowManager *windowManager) {
	_windowManager = windowManager;
}

void Environment::setResourceManager(ResourceManager *resourceManager) {
	_resourceManager = resourceManager;
}

void Environment::setScriptManager(ScriptManager *scriptManager) {
	_scriptManager = scriptManager;
}

void Environment::setInputManager(InputManager *inputManager) {
	_inputManager = inputManager;
}

Clock *Environment::getClock() {
	return _clock;
}

WindowManager *Environment::getWindowManager() {
	return _windowManager;
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