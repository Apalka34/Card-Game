#include "cprocessing.h"
#include "Defaults.h"
#include "DynamicMemoryManager.h"
#include "Renderer.h"

UIElement* GetLastChild(UIElement element) {
	if (!element.firstChild) {
		return NULL;
	}

	UIElement* child = element.firstChild;
	while (child->nextSibling) child = child->nextSibling;
	return child;
}

// Dont use function so i can kinda treat it like range operator
#define ELE_F(X) element.initialParameters[##X].f + element.parameterChanges[##X].f(element)
#define ELE_I(X) element.initialParameters[##X].i + element.parameterChanges[##X].i(element)
#define ELE_IMG(X) element.initialParameters[##X].image
#define ELE_T(X) element.initialParameters[##X].text
#define NULL_COALESCING(A, B) A ? A : B
void DrawUIElement(UIElement element) {
	if (!element.currentlyEnabled) return;

	if (element.settings.drawColor.a != 0) CP_Settings_Fill(element.settings.drawColor);
	else CP_Settings_NoFill();
	if (element.settings.strokeColor.a != 0) CP_Settings_Stroke(element.settings.strokeColor);
	else CP_Settings_NoStroke();
	CP_Settings_StrokeWeight(NULL_COALESCING(element.settings.strokeWeight, DEFAULT_STROKE_WEIGHT));
	CP_Settings_LineCapMode(NULL_COALESCING(element.settings.lineCapMode, DEFAULT_LINE_CAP_MODE));
	CP_Settings_LineJointMode(NULL_COALESCING(element.settings.lineJointMode, DEFAULT_LINE_JOINT_MODE));
	CP_Settings_TextSize(NULL_COALESCING(element.settings.textSize, DEFAULT_TEXT_SIZE));
	CP_Settings_TextAlignment(NULL_COALESCING(element.settings.textAlignH, DEFAULT_TEXT_ALIGN_H), NULL_COALESCING(element.settings.textAlignV, DEFAULT_TEXT_ALIGN_V));
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_TOP);

	switch (element.functionVariant) { // this is also horrendous
		case F2:
			element.draw.f2(ELE_F(0), ELE_F(1));
			break;
		case F3:
			element.draw.f3(ELE_F(0), ELE_F(1), ELE_F(2));
			break;
		case F4:
			element.draw.f4(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3));
			break;
		case F5:
			element.draw.f5(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4));
			break;
		case F6:
			element.draw.f6(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5));
			break;
		case F7:
			element.draw.f7(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5), ELE_F(6));
			break;
		case F8:
			element.draw.f8(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5), ELE_F(6), ELE_F(7));
			break;
		case F9:
			element.draw.f9(ELE_F(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5), ELE_F(6), ELE_F(7), ELE_F(8));
			break;
		case IMG:
			element.draw.img(ELE_IMG(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_I(5));
			break;
		case IMG_A:
			element.draw.imgA(ELE_IMG(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_I(5), ELE_F(6));
			break;
		case IMG_S:
			element.draw.imgS(ELE_IMG(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5), ELE_F(6), ELE_F(7), ELE_F(8), ELE_I(9));
			break;
		case IMG_SA:
			element.draw.imgSA(ELE_IMG(0), ELE_F(1), ELE_F(2), ELE_F(3), ELE_F(4), ELE_F(5), ELE_F(6), ELE_F(7), ELE_F(8), ELE_I(9), ELE_F(10));
			break;
		case TXT:
			element.draw.txt(ELE_T(0), ELE_F(1), ELE_F(2));
			break;
		case TXT_B:
			element.draw.txtB(ELE_T(0), ELE_F(1), ELE_F(2), ELE_F(3));
			break;
	}

	// goes down the hierchy going down all the children and then their siblings
	UIElement* child = element.firstChild;
	while (child) {
		DrawUIElement(*child);
		child = child->nextSibling;
	}
}

UIElement* CreateUIElement(UIElement* parent, DrawSettings settings, union DrawFunction draw, enum FunctionVariant functionVariant, int paramCount, union DrawParam initialParameters[11], union DrawParamChange parameterChanges[11]) {
	UIElement* uiElement = (UIElement*)CreateDynamicVariable(sizeof(UIElement));
	uiElement->currentlyEnabled = 1;
	uiElement->settings = settings;
	uiElement->functionVariant = functionVariant;
	uiElement->draw = draw;
	memcpy(uiElement->initialParameters, initialParameters, paramCount * sizeof(union DrawParam));
	memcpy(uiElement->parameterChanges, parameterChanges, paramCount * sizeof(union DrawParamChange));

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