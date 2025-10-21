#include "cprocessing.h"
#include "InputManager.h"

struct CP_ANY CreateCP_ANY(enum InputType inputType, int number) {
	return (struct CP_ANY){
		.inputType = inputType,
		.value = number
	};
}
static CP_BOOL KeyInputTrigger(int value) { return CP_Input_KeyTriggered(value); }
static CP_BOOL KeyMouseTrigger(int value) { return CP_Input_MouseTriggered(value); }
static CP_BOOL KeyGamepadTrigger(int value) { return CP_Input_GamepadTriggered(value); }
CP_BOOL(*func[])(int) = { KeyInputTrigger, KeyMouseTrigger, KeyGamepadTrigger };
void SetCP_ANY(struct CP_ANY* cp_any, enum InputType inputType, int number) {
	cp_any->inputType = inputType;
	cp_any->value = number;
}
int CP_Input_Triggered(struct CP_ANY cp_any) {
	return func[(int)cp_any.inputType](cp_any.value);
}

/// <summary>
/// Gets user input for numbers
/// </summary>
/// <param name=""></param>
/// <returns>The number that the user pressed, with 0 as 10. Returns 0 if no number was pressed this frame.</returns>
int GetCardInput(void) {
	CP_KEY min = KEY_0;
	CP_KEY max = KEY_9;
	int result = 0;

	for (int i = min; i <= max; i++) {
		if (CP_Input_KeyDown(i)) {
			result = i - min;
			if (result == 0) result = max - min + 1; // converts 0 to 10 as 0 is rightmost key on keyboards AND 0 is used for no input
			break;
		}
	}

	return result;
}