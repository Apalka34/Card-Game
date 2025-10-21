#pragma once

enum InputType { CP_KEY_INPUT, CP_MOUSE_INPUT, CP_GAMEPAD_INPUT };
struct CP_ANY {
	enum InputType inputType;
	int value;

};
struct CP_ANY CreateCP_ANY(enum InputType inputType, int number);
void SetCP_ANY(struct CP_ANY* cp_any, enum InputType inputType, int number);
int CP_Input_Triggered(struct CP_ANY cp_any);
int GetCardInput(void);