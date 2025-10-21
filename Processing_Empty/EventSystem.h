#pragma once

#include "InputManager.h"

typedef void (*Function)(/*void* payload*/);
typedef void* (*Payload)(/*void*/);
struct InputEvent {
	struct CP_ANY originalInput;
	struct CP_ANY input;
	Function eventTarget;
	Payload payload;
};

int AttemptFirePayload(struct InputEvent event);
struct InputEvent CreateInputEvent(struct CP_ANY originalInput, Function target, Payload payload);