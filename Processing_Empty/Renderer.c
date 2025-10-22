#include "cprocessing.h"
#include "Renderer.h"

UIElement* GetLastChild(UIElement element) {
	if (!element.firstChild) {
		return NULL;
	}

	UIElement* child = element.firstChild;
	while (child->nextSibling) child = child->nextSibling;
	return child;
}

#define ELEMENT(X) element.initialParameters[##X] // Dont use function so i can kinda treat it like range operator
void DrawUIElement(UIElement element) {
	if (!element.currentlyEnabled) return;
	switch (element.parameterCount) { // this is also horrendous
		case 2:
			element.draw.f2(ELEMENT(0), ELEMENT(1));
			break;
		case 3:
			element.draw.f3(ELEMENT(0), ELEMENT(1), ELEMENT(2));
			break;
		case 4:
			element.draw.f4(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3));
			break;
		case 5:
			element.draw.f5(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3), ELEMENT(4));
			break;
		case 6:
			element.draw.f6(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3), ELEMENT(4), ELEMENT(5));
			break;
		case 7:
			element.draw.f7(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3), ELEMENT(4), ELEMENT(5), ELEMENT(6));
			break;
		case 8:
			element.draw.f8(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3), ELEMENT(4), ELEMENT(5), ELEMENT(6), ELEMENT(7));
			break;
		case 9:
			element.draw.f9(ELEMENT(0), ELEMENT(1), ELEMENT(2), ELEMENT(3), ELEMENT(4), ELEMENT(5), ELEMENT(6), ELEMENT(7), ELEMENT(8));
			break;
	}

	UIElement* child = element.firstChild;
	while (child) {
		DrawUIElement(*child);
		child = child->nextSibling;
	}
}

UIElement* CreateUIElement(UIElement* parent, union DrawFunction draw, int parameterCount, float initialParameters[9]) {
	UIElement* uiElement = (UIElement*)CreateDynamicVariable(sizeof(UIElement));
	uiElement->currentlyEnabled = 1;
	uiElement->parameterCount = parameterCount;
	uiElement->draw = draw;
	memcpy(uiElement->initialParameters, initialParameters, parameterCount * sizeof(float));

	uiElement->parent = parent;
	if (uiElement->parent) {
		uiElement->siblingIndex = parent->childCount;
		UIElement* parentsLastChild = GetLastChild(*uiElement->parent);
		if (parentsLastChild) parentsLastChild->nextSibling = uiElement;
		else uiElement->parent->firstChild = uiElement;
		uiElement->parent->childCount++;
	}
	else uiElement->siblingIndex = 0;
	uiElement->childCount = 0;
	uiElement->firstChild = NULL;
	uiElement->nextSibling = NULL;

	return uiElement;
}