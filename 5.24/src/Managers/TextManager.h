#include <vector>

#include "Text.h"

#ifndef TEXT_MANAGER_H
#define TEXT_MANAGER_H

class TextManager {
public:
	TextManager();
	~TextManager();
	void update_text();
	void add_text(Text *text);
	void remove_text(Text *text);
	void render_text();
private:
	std::vector<Text *> _texts;
};

#endif