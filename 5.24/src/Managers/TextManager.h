#include <vector>

#include "Text.h"
#include "TextBox.h"

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

class TextManager {
public:
	TextManager();
	~TextManager();
	//void update_text();
	void add_text(Text *text);
	void remove_text(Text *text);
	void render_text();

	TextBox &get_textbox() { return _textbox; }

	friend class ResourceManager;
private:
	std::vector<Text *> _texts;

	TextBox _textbox;
};

#endif