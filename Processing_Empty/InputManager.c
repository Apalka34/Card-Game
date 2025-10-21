#include <assert.h>
#include "cprocessing.h"
#include "InputManager.h"

/// <summary>
/// Creates a CP_ANY struct instance, in order to be able to get values for an input
/// </summary>
/// <param name="keyValue">Take from CP_KEY. -1 if this ignores keyboard input</param>
/// <param name="mouseValue">Take from CP_MOUSE. -1 if this ignores mouse input</param>
/// <param name="gamepadValue">Take from CP_GAMEPAD. -1 if this ignores gamepad input</param>
/// <returns></returns>
struct CP_ANY CreateCP_ANY(int keyValue, int mouseValue, int gamepadValue)
{
	assert((keyValue == -1) != (mouseValue == -1)); // keyValue and mouseValue must be exclusive as they share a input scheme
	return (CP_ANY)
	{
		.usesMouse = (keyValue == -1),
		.inputIDs = { keyValue, mouseValue, gamepadValue }
	};
}
static CP_BOOL KeyInputTrigger(int value) { return CP_Input_KeyTriggered(value); }
static CP_BOOL KeyMouseTrigger(int value) { return CP_Input_MouseTriggered(value); }
static CP_BOOL KeyGamepadTrigger(int value) { return CP_Input_GamepadTriggered(value); }
static int InputIndex(CP_ANY cp_any) { return CurrentInputScheme == KEYBOARD_MOUSE ? cp_any.usesMouse : CurrentInputScheme; }
CP_BOOL(*GetFromInputDevice[])(int) = { KeyInputTrigger, KeyMouseTrigger, KeyGamepadTrigger };
void SetCP_ANY(struct CP_ANY* cp_any, enum InputSource inputType, int number)
{
	cp_any->inputIDs[inputType] = number;
	if (inputType <= CP_MOUSE_INPUT) cp_any->usesMouse = inputType;
}
int CP_Input_Triggered(CP_ANY cp_any) { return GetFromInputDevice[InputIndex(cp_any)](cp_any.inputIDs[InputIndex(cp_any)]); }

static int AnyDeviceInput(int deviceIndex, int min, int max) {
	for (int i = min; i < max; i++) {
		if (GetFromInputDevice[deviceIndex](i)) {
			return 1;
		}
	}
	return 0;
}
static int AnyMouseInput(void) { return AnyDeviceInput(CP_MOUSE_INPUT, MOUSE_BUTTON_1, MOUSE_BUTTON_8); }
static int AnyGamepadInput(void) { return AnyDeviceInput(CP_GAMEPAD_INPUT, GAMEPAD_DPAD_UP, GAMEPAD_Y); }
int UpdateCurrentDevice(void) {
	InputScheme currentScheme = CurrentInputScheme;

	if (CP_Input_KeyTriggered(KEY_ANY) || AnyMouseInput()) { CurrentInputScheme = KEYBOARD_MOUSE; }
	if (AnyGamepadInput()) { CurrentInputScheme = GAMEPAD; }

	return currentScheme == CurrentInputScheme;
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