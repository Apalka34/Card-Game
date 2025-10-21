#pragma once

#include "cprocessing.h"

typedef enum InputScheme { KEYBOARD_MOUSE = 0, GAMEPAD = 2 } InputScheme; // 0 vs 1 gets handled by usesMouse
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


extern float StickDeadzone;
/// <summary>
/// Gets whether the input CP_Vector from the stick should get get ignored by basis of both
/// the x and y value being lower than or equal to the StickDeadzone threshold
/// </summary>
/// <param name="stick">The input CP_Vector of the stick</param>
/// <returns>1 if stick should be ignored; 0 if stick should not be ignored</returns>
static int HasStickInput(CP_Vector stickValue) {
	float xAbs = stickValue.x < 0 ? -stickValue.x : stickValue.x;
	float yAbs = stickValue.y < 0 ? -stickValue.y : stickValue.y;
	return xAbs > StickDeadzone || yAbs > StickDeadzone;
}