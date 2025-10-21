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

InputScheme CurrentInputScheme = KEYBOARD_MOUSE;
float StickDeadzone = 0.075f;

#define REBINDEABLE_INPUT_EVENTS 1
struct InputEvent inputEvents[REBINDEABLE_INPUT_EVENTS];
void EscapeCurrent() {
	DebugPrintf("Escaped through input source %i\n", CurrentInputScheme);
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
	
	//CP_System_FullscreenAdvanced(1920, 1080);
	memcpy(inputEvents, (InputEvent[]){ // I prefer this than setting by index such as commented line below
		CreateInputEvent(CreateCP_ANY(KEY_ESCAPE, -1, GAMEPAD_B), EscapeCurrent)
	}, sizeof(inputEvents));
	//inputEvents[0] = CreateInputEvent(CreateCP_ANY(KEY_ESCAPE, -1, GAMEPAD_B), EscapeCurrent);
	//SetCP_ANY(&inputEvents[0].input, CP_MOUSE_INPUT, MOUSE_BUTTON_RIGHT); | Rebind example
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
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void game_exit(void)
{
	// shut down the gamestate and cleanup any dynamic memory
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