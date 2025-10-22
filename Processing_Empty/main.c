//---------------------------------------------------------
// file:	main.c
// author:	[NAME]
// email:	[DIGIPEN EMAIL ADDRESS]
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "DebugUtil.h"
#include "EventSystem.h"
#include "InputManager.h"
#include "Renderer.h"
#include "assert.h"

UIElement* circle;
UIElement* rect;

InputScheme CurrentInputScheme = KEYBOARD_MOUSE;
float StickDeadzone = 0.075f;

#define REBINDEABLE_INPUT_EVENTS 4
struct InputEvent inputEvents[REBINDEABLE_INPUT_EVENTS];
void EscapeCurrent() {
	DebugPrintf("Escaped through input source %i\n", CurrentInputScheme);
	CP_Engine_Terminate();
}
void ToggleCircle()
{
	int current = circle->currentlyEnabled;
	circle->currentlyEnabled = !circle->currentlyEnabled;
	DebugPrintf("circle.currentlyEnabled went from %i to %i\n", current, circle->currentlyEnabled);
}
void ToggleRect()
{
	int current = rect->currentlyEnabled;
	rect->currentlyEnabled = !rect->currentlyEnabled;
	DebugPrintf("circle.currentlyEnabled went from %i to %i\n", current, rect->currentlyEnabled);
}
void ToggleTrig() {
	UIElement* trig = GetLastChild(*circle);
	int current = trig->currentlyEnabled;
	trig->currentlyEnabled = !trig->currentlyEnabled;
	DebugPrintf("circle.currentlyEnabled went from %i to %i\n", current, trig->currentlyEnabled);
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
	
	//CP_System_FullscreenAdvanced(1920, 1080);
	memcpy(inputEvents, (InputEvent[]){ // I prefer this than setting by index such as commented line below
		CreateInputEvent(CreateCP_ANY(KEY_ESCAPE, -1, GAMEPAD_B), EscapeCurrent),
		CreateInputEvent(CreateCP_ANY(KEY_C, -1, -1), ToggleCircle),
		CreateInputEvent(CreateCP_ANY(KEY_R, -1, -1), ToggleRect),
		CreateInputEvent(CreateCP_ANY(KEY_T, -1, -1), ToggleTrig)
	}, sizeof(inputEvents));
	//inputEvents[0] = CreateInputEvent(CreateCP_ANY(KEY_ESCAPE, -1, GAMEPAD_B), EscapeCurrent);
	//SetCP_ANY(&inputEvents[0].input, CP_MOUSE_INPUT, MOUSE_BUTTON_RIGHT); | Rebind example

	circle = CreateUIElement(NULL, (DrawFunction){.f3 = CP_Graphics_DrawCircle }, 3,
							  (float[]) { CP_System_GetWindowWidth() / 2.F, CP_System_GetWindowHeight() / 2.F, 50.F });
	rect = CreateUIElement(circle, (DrawFunction){.f6 = CP_Graphics_DrawRectAdvanced }, 6,
						   (float[]) { 250.F, 250.F, 50.F, 25.F, 135.F, 15.F });
	DebugPrintf("rect (%p) should be in circle (%p)\n", rect, (*circle).firstChild);
	CreateUIElement(circle, (DrawFunction){ .f6 = CP_Graphics_DrawTriangle }, 6,
					(float[]) { 150.F, 250.F, 200.F, 200.F, 150.F, 150.F });
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.

	//int cardInput = GetCardInput();
	if (UpdateCurrentDevice()) {
		DebugPrintf("Changed to input device %i\n", CurrentInputScheme);
	}
	for (int i = 0; i < REBINDEABLE_INPUT_EVENTS; i++) { AttemptInputEvent(inputEvents[i]); }

	CP_Graphics_ClearBackground(CP_Color_Create(0, 0, 0, 1));
	DrawUIElement(*circle);
	//DrawUIElement(rect);
	//CP_Graphics_DrawCircle(CP_System_GetWindowWidth() / 2.F, CP_System_GetWindowHeight() / 2.F, 50.F);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	DebugPrintf("game exiting\n");
	FreeDynamicVariables();
}

// main() the starting point for the program
// CP_Engine_SetNextGameState() tells CProcessing which functions to use for init, update and exit
// CP_Engine_Run() is the core function that starts the simulation
int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}