#pragma once

#include "InputManager.h"

typedef void (*Function)(/*void* payload*/);
typedef struct InputEvent {
	CP_ANY originalInput;
	CP_ANY input;
	Function eventTarget;
} InputEvent;

int AttemptInputEvent(InputEvent event);
InputEvent CreateInputEvent(CP_ANY originalInput, Function target);