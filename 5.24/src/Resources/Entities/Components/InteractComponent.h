#include "Component.h"

#include <string>

#ifndef INTERACT_COMPONENT_H
#define INTERACT_COMPONENT_H

struct InteractComponent : public Component {
	InteractComponent(Entity *entity_, std::string name_, std::string script_name_, std::string script_, std::string ui_info_);
	InteractComponent(Entity *new_entity, const InteractComponent &rhs);
	InteractComponent *copy(Entity *new_entity) const;

	void update();

	void interact();

	const int get_type() const { return COMPONENT_INTERACT; }

	std::string name;
	std::string script_name;
	std::string script;
	std::string ui_info;

	int special_id;
};

#endif