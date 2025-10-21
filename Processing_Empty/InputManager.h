#pragma once

typedef enum InputScheme { KEYBOARD_MOUSE = 0, GAMEPAD = 2 } InputScheme;
extern InputScheme CurrentInputScheme;
typedef enum InputSource { CP_KEY_INPUT, CP_MOUSE_INPUT, CP_GAMEPAD_INPUT } InputSource;
typedef struct CP_ANY
{
	int usesMouse; // for keyboard mouse scheme.
	int inputIDs[3];
} CP_ANY;

CP_ANY CreateCP_ANY(int keyValue, int mouseValue, int gamepadValue);
void SetCP_ANY(CP_ANY* cp_any, enum InputSource inputType, int value);
int CP_Input_Triggered(CP_ANY cp_any);
int UpdateCurrentDevice(void);

int GetCardInput(void);