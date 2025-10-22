#pragma once

#include "DynamicMemoryManager.h"

// Well this is horrendous
static float Float0(void) { return 0.F; }
typedef union DrawFunction {
	void (*f2)(float, float); // 2
	void (*f3)(float, float, float); // 3
	void (*f4)(float, float, float, float); // 4
	void (*f5)(float, float, float, float, float); // 5
	void (*f6)(float, float, float, float, float, float); // 6
	void (*f7)(float, float, float, float, float, float, float); // 7
	void (*f8)(float, float, float, float, float, float, float, float); // 8
	void (*f9)(float, float, float, float, float, float, float, float, float); // 9
} DrawFunction;
typedef struct UIElement {
	// core data
	int currentlyEnabled;
	int parameterCount;
	union DrawFunction draw;
	float initialParameters[9];

	// related UI elements
	int childCount;
	int siblingIndex;
	struct UIElement* parent;
	struct UIElement* firstChild;
	struct UIElement* nextSibling;
} UIElement;

UIElement* GetLastChild(UIElement element);

void DrawUIElement(UIElement element);

UIElement* CreateUIElement(UIElement* parent, union DrawFunction draw, int parameterCount, float initialParameters[9]);