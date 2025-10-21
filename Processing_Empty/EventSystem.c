#include "cprocessing.h"
#include "EventSystem.h"
#include "InputManager.h"

int AttemptInputEvent(struct InputEvent event) {
	if (CP_Input_Triggered(event.input)) {
		event.eventTarget(/*event.payload()*/);
		return 1;
	}
	return 0;
}

struct InputEvent CreateInputEvent(CP_ANY originalInput, Function target) {
	return (InputEvent) {
		.originalInput = originalInput,
		.input = originalInput,
		.eventTarget = target,
	};
}