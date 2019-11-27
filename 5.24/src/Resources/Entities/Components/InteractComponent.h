#include "Component.h"

#include <SDL_rect.h>

#include "Text.h"

#include <string>

#ifndef INTERACT_COMPONENT_H
#define INTERACT_COMPONENT_H

bool interact_in_range(const SDL_Rect &a, const SDL_Rect &b);

struct InteractComponent : public Component {
	InteractComponent(Entity *entity_, std::string name_, std::string script_name_, std::string script_, std::string ui_info_);
	InteractComponent(Entity *new_entity, const InteractComponent &rhs);
	InteractComponent *copy(Entity *new_entity) const;

	void update();

	void interact();

	void display_info();

	const int get_type() const { return COMPONENT_INTERACT; }

	std::string name;
	std::string script_name;
	std::string script;
	std::string ui_info;

	std::string data;

	Text info_text;
};

#endif