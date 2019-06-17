#include "Environment.h"
#include "Clock.h"
#include "LogManager.h"
#include "WindowManager.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "InputManager.h"

Environment *Environment::_instance = nullptr;

Environment::Environment() :
	_clock					( nullptr ),
	_logManager				( nullptr ),
	_windowManager			( nullptr ),
	_scriptManager			( nullptr ),
	_resourceManager		( nullptr ),
	_inputManager			( nullptr )
{
	assert(!_instance);
	_instance = this;
}

Environment::~Environment() {
	shutdown();
	_instance = nullptr;
}

void Environment::shutdown() {
	delete _inputManager;
	_inputManager = nullptr;

	delete _resourceManager;
	_resourceManager = nullptr;

	delete _scriptManager;
	_scriptManager = nullptr;

	delete _windowManager;
	_windowManager = nullptr;

	delete _logManager;
	_logManager = nullptr;

	delete _clock;
	_clock = nullptr;
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