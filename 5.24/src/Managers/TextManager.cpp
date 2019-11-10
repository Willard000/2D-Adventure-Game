#include "TextManager.h"

#include "Environment.h"
#include "Window.h"

#define TEXTBOX_HEIGHT 200
#define TEXTBOX_COLOR { 50, 50, 50, 50 }

#define WINDOW_WIDTH Environment::get().get_window()->get_width()
#define WINDOW_HEIGHT Environment::get().get_window()->get_height()

TextManager::TextManager() :
	_textbox{ {0, WINDOW_HEIGHT - TEXTBOX_HEIGHT, WINDOW_WIDTH / 3, TEXTBOX_HEIGHT}, TEXTBOX_COLOR }
{}

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