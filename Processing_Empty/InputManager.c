#include <assert.h>
#include "cprocessing.h"
#include "DebugUtil.h"
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
static CP_BOOL MouseInputTrigger(int value) { return CP_Input_MouseTriggered(value); }
static CP_BOOL GamepadInputTrigger(int value) { return CP_Input_GamepadTriggered(value); }
static int InputIndex(CP_ANY cp_any) { return CurrentInputScheme == KEYBOARD_MOUSE ? cp_any.usesMouse : CurrentInputScheme; }
CP_BOOL(*GetFromInputDevice[])(int) = { KeyInputTrigger, MouseInputTrigger, GamepadInputTrigger };
void SetCP_ANY(struct CP_ANY* cp_any, enum InputSource inputType, int number)
{
	cp_any->inputIDs[inputType] = number;
	if (inputType <= CP_MOUSE_INPUT) cp_any->usesMouse = inputType;
}
int CP_Input_Triggered(CP_ANY cp_any)
{
	int index = InputIndex(cp_any);
	if (cp_any.inputIDs[index] == -1) return 0; // disabled, must be wrong. No reason to check.
	return GetFromInputDevice[index](cp_any.inputIDs[index]);
}

static int DeviceTriggered(int deviceIndex, int min, int max) {
	for (int i = min; i <= max; i++) {
		if (GetFromInputDevice[deviceIndex](i)) {
			return i;
		}
	}
	return -1; // MOUSE and GAMEPAD start at 0. 0 should be reserved for no input. rude
}
static int KeyTriggered(void)
{
	if (!CP_Input_KeyTriggered(KEY_ANY)) return 0; // we know no input was triggered

	int indexes[] = { // indexes of all keys excluding gaps
		KEY_SPACE,			KEY_SPACE,			// 32  -> 32
		KEY_APOSTROPHE,		KEY_APOSTROPHE,		// 39  -> 39
		KEY_COMMA,			KEY_9,				// 44  -> 57
		KEY_SEMICOLON,		KEY_SEMICOLON,		// 59  -> 59
		KEY_EQUAL,			KEY_EQUAL,			// 61  -> 61
		KEY_A,				KEY_RIGHT_BRACKET,	// 65  -> 93
		KEY_GRAVE_ACCENT,	KEY_GRAVE_ACCENT,	// 96  -> 96
		KEY_WORLD_1,		KEY_WORLD_2,		// 161 -> 162
		KEY_ESCAPE,			KEY_END,			// 256 -> 269
		KEY_CAPS_LOCK,		KEY_PAUSE,			// 280 -> 284
		KEY_F1,				KEY_F25,			// 290 -> 314
		KEY_KP_0,			KEY_KP_EQUAL,		// 320 -> 336
		KEY_LEFT_SHIFT,		KEY_MENU			// 340 -> 348
	};
	int elements = sizeof(indexes) / sizeof(int);
	int sectionIndex = 0;
	int i = indexes[0];

	while (1) {
		// Skips to next section to not run code on non existing keys
		if (i > indexes[sectionIndex + 1]) {
			sectionIndex += 2;
			if (sectionIndex >= elements) return 0;
			i = indexes[sectionIndex];
		}

		if (KeyInputTrigger(i)) return i;
		i++;
	}

	assert(0); // an input was triggered and we dont have it. Something is going wrong
}
static int MouseTriggered(void)
{
	if (CP_Input_MouseMoved() || CP_Input_MouseWheel()) return 1;
	return DeviceTriggered(CP_MOUSE_INPUT, MOUSE_BUTTON_1, MOUSE_BUTTON_LAST) != -1;
}
static int GamepadTriggered(void)
{
	int hasTriggerInput = CP_Input_GamepadLeftTrigger() || CP_Input_GamepadRightTrigger();
	int hasStickInput = HasStickInput(CP_Input_GamepadLeftStick()) || HasStickInput(CP_Input_GamepadRightStick());
	if (hasTriggerInput || hasStickInput) return 1;
	return DeviceTriggered(CP_GAMEPAD_INPUT, GAMEPAD_DPAD_UP, GAMEPAD_Y) != -1;
}
static int AttemptUpdateCurrentDevice(InputScheme compareScheme, int inputResult) {
	if (CurrentInputScheme != compareScheme && inputResult) {
		CurrentInputScheme = compareScheme;
		return 1;
	}
	return 0;
}
int UpdateCurrentDevice(void) {
	if (AttemptUpdateCurrentDevice(KEYBOARD_MOUSE, KeyTriggered() || MouseTriggered())) return 1;
	else if (AttemptUpdateCurrentDevice(GAMEPAD, GamepadTriggered())) return 1;
	return 0;
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