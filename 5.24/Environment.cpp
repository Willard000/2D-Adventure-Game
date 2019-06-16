#include "Environment.h"
#include "Clock.h"
#include "LogManager.h"
#include "WindowManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

Environment *Environment::_instance = nullptr;

Environment::Environment()
	: _clock(0),
	_logManager(0),
	_windowManager(0),
	_scriptManager(0),
	_resourceManager(0),
	_inputManager(0)
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {
	shutdown();
	_instance = 0;
}

void Environment::shutdown() {
	delete _inputManager;
	_inputManager = 0;

	delete _resourceManager;
	_resourceManager = 0;

	delete _scriptManager;
	_scriptManager = 0;

	delete _windowManager;
	_windowManager = 0;

	delete _logManager;
	_logManager = 0;

	delete _clock;
	_clock = 0;
}

Environment &Environment::get(){
	assert(_instance);
	return *_instance;
}

void Environment::setClock(Clock *clock) {
	_clock = clock;
}

void Environment::setLogManager(LogManager *logManager) {
	_logManager = logManager;
}

void Environment::setWindowManager(WindowManager *windowManager) {
	_windowManager = windowManager;
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

Clock *Environment::getClock() {
	return _clock;
}

LogManager *Environment::getLogManager() {
	return _logManager;
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