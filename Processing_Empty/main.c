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

#define REBINDEABLE_INPUT_EVENTS 1
struct InputEvent inputEvents[REBINDEABLE_INPUT_EVENTS];
int hadInput;
void GotAnInput(/*void* parameters*/) {
	hadInput = 1;
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void game_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
	//CP_System_FullscreenAdvanced(1920, 1080);
	inputEvents[0] = CreateInputEvent(CreateCP_ANY(CP_KEY_INPUT, KEY_ESCAPE), GotAnInput, 0);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void game_update(void)
{
	// check input, update simulation, render etc.
	hadInput = 0;
	int hadInputInternal = 0;

	int cardInput = GetCardInput();
	for (int i = 0; i < REBINDEABLE_INPUT_EVENTS; i++) {
		if (AttemptFirePayload(inputEvents[i])) {
			hadInputInternal = 1;
			SetCP_ANY(&inputEvents[0].input, CP_MOUSE_INPUT, MOUSE_BUTTON_LEFT);
		}
	}
	
	DebugPrintf("RAN LOOP: %i | %i -> %i\n", cardInput, hadInput, hadInputInternal);
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