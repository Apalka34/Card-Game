#include "cprocessing.h"
#include "EventSystem.h"
#include "InputManager.h"

int AttemptFirePayload(struct InputEvent event) {
	if (CP_Input_Triggered(event.input)) {
		event.eventTarget(/*event.payload()*/);
		return 1;
	}
	return 0;
}

struct InputEvent CreateInputEvent(struct CP_ANY originalInput, Function target, Payload payload) {
	return (struct InputEvent) {
		.originalInput = originalInput,
		.input = originalInput,
		.eventTarget = target,
		.payload = payload
	};
}