#include "TextManager.h"

#include "Environment.h"
#include "Window.h"

TextManager::TextManager() {}

TextManager::~TextManager() {
	for (auto &t : _texts) {
		delete t;
	}
}

void TextManager::update_text() {
	auto it = _texts.begin();
	while (it != _texts.end()) {
		(*it)->update();

		if ((*it)->end()) {
			delete *it;
			it = _texts.erase(it);
		}
		else {
			++it;
		}
	}
}

void TextManager::add_text(Text *text) {
	_texts.push_back(text);
}

void TextManager::remove_text(Text *text) {
	for (auto it = _texts.begin(); it != _texts.end(); ++it) {
		if (*it == text) {
			delete *it;
			_texts.erase(it);
		}
	}
}

void TextManager::render_text() {
	for (auto &t : _texts) {
		Environment::get().get_window()->get_renderer()->draw_text(t);
	}
}