#include "UIHandler_Editor.h"

UIHandler_Editor::UIHandler_Editor(){
	addButton(this, &UIHandler_Editor::button_new_map);
}

#include <stdio.h>
void UIHandler_Editor::button_new_map() {
	printf("hi");
}