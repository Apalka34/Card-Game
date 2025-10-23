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
#include "Defaults.h"
#include "DynamicMemoryManager.h"
#include "EventSystem.h"
#include "InputManager.h"
#include "Renderer.h"
#include "assert.h"

UIElement* circle;
UIElement* rect;

#define REBINDEABLE_INPUT_EVENTS 4
struct InputEvent inputEvents[REBINDEABLE_INPUT_EVENTS];
void EscapeCurrent() {
	DebugPrintf("Escaped through input source %i\n", CurrentInputScheme);
	CP_Engine_Terminate();
}
void ToggleCircle() { circle->currentlyEnabled = !circle->currentlyEnabled; }
void ToggleRect() { rect->currentlyEnabled = !rect->currentlyEnabled; }
void ToggleTrig() {
	UIElement* trig = GetLastChild(*circle);
	trig->currentlyEnabled = !trig->currentlyEnabled;
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

	circle = CreateUIElement(NULL, (DrawSettings) { .drawColor = CP_Color_Create(255, 0, 0, 255), .textAlignH = CP_TEXT_ALIGN_H_CENTER }, (DrawFunction) { .f3 = CP_Graphics_DrawCircle }, F3, 3,
							 (DrawParam[]) { CP_System_GetWindowWidth() / 2.F, CP_System_GetWindowHeight() / 2.F, 50.F },
							 (DrawParamChange[]) { Float0, Float0, Float0 });
	rect = CreateUIElement(circle, (DrawSettings) { .drawColor = CP_Color_Create(0, 255, 0, 255) }, (DrawFunction) { .f6 = CP_Graphics_DrawRectAdvanced }, F6, 6,
						   (DrawParam[]) { 250.F, 250.F, 50.F, 25.F, 125.F, 15.F },
						   (DrawParamChange[]) { Float0, Float0, Float0, Float0, Float0, Float0 });
	CreateUIElement(circle, (DrawSettings) { .drawColor = CP_Color_Create(0, 0, 255, 255) }, (DrawFunction) { .f6 = CP_Graphics_DrawTriangle }, F6, 6,
					(DrawParam[]) { 150.F, 250.F, 200.F, 200.F, 150.F, 150.F },
					(DrawParamChange[]) { Float0, Float0, Float0, Float0, Float0, Float0 });
	CreateUIElement(rect, (DrawSettings) { NO_COLOR }, (DrawFunction) { .img = CP_Image_Draw }, IMG, 6,
					(DrawParam[]) { { .image = CreateDynamicImage("./Assets/DigiPen_RED.png") }, CP_System_GetWindowWidth() / 2.F, CP_System_GetWindowHeight() / 2.F, 1026.F / 4.F, 249.F / 4.F, {.i = 200} },
					(DrawParamChange[]) { { .none = NO_INPUT }, Float0, Float0, Float0, Float0, { .i = Int0 } });
	CreateUIElement(circle, (DrawSettings) { .drawColor = CP_Color_Create(255, 0, 255, 128), .strokeColor = CP_Color_Create(0, 255, 0, 255) }, (DrawFunction) { .txtB = CP_Font_DrawTextBox }, TXT_B, 4,
					(DrawParam[]) { { .text = "My Functionality Example" }, CP_System_GetWindowWidth() / 2.F - 50.F, 20.F, 100.F },
					(DrawParamChange[]) { { .none = NO_INPUT }, Float0, Float0, Float0 });
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

	CP_Graphics_ClearBackground(CP_Color_Create(255, 255, 255, 255));
	DrawUIElement(*circle);
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
	DebugPrintf("game exiting\n");
	FreeAllDynamicVariables();
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