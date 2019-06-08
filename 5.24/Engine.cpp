#include "Engine.h"

Engine::Engine() {
	_system = std::make_shared<System>(true);
	_clock = std::make_shared<Clock>(-1);
	_resourceManager = std::make_shared<ResourceManager>(_system->getWindow()->getRenderer());
	_inputHandler = std::make_shared<InputHandler>();
	_input = std::make_shared<Input>(
		_system,
		_clock,
		_inputHandler,
		_resourceManager->getPlayer()
	);

	_isRunning = true;
}

void Engine::run() {
	while (_isRunning) {
		_isRunning = _input->get();

		_input->update();
		_resourceManager->updateEntities();
		_resourceManager->render();

		if (_clock->update()) {
			updateTitle();
		}
	}
}

void Engine::updateTitle() {
	static std::string title;
	title = WINDOW_TITLE + " fms: " + std::to_string(_clock->getFMS());
	_system->getWindow()->setTitle(title);
}