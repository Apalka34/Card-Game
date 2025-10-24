#include <assert.h>
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

#define ELE(INDEX, NAME) element.initialParameters[INDEX].NAME + element.parameterChanges[INDEX].NAME(element)
#define CONST_ELE(INDEX, NAME) element.initialParameters[INDEX].NAME
#define NULL_COALESCING(A, B) A ? A : B
#define RANGE(MIN, VAR, MAX) MIN <= VAR && VAR <= MAX
#define SETTINGF(REQ, FUNC, VAR, VARCHECK, FUNC2) if(REQ){if(VAR##VARCHECK){FUNC(VAR);}else{FUNC2();}}else{assert(!VAR##VARCHECK);}
#define SETTINGV(REQ, FUNC, VAR, DEF) if(REQ){FUNC(NULL_COALESCING(VAR,DEF));}else{assert(!VAR);}

void DrawUIElement(UIElement element) {
	if (!element.currentlyEnabled) return;

	SETTINGF(element.functionVariant <= TXT_B, CP_Settings_Fill, element.settings.drawColor, .a, CP_Settings_NoFill);
	SETTINGF(element.functionVariant <= F9, CP_Settings_Stroke, element.settings.strokeColor, .a, CP_Settings_NoStroke);
	SETTINGV(element.functionVariant <= F9, CP_Settings_StrokeWeight, element.settings.strokeWeight, DEFAULT_STROKE_WEIGHT);
	SETTINGV(element.functionVariant <= F9, CP_Settings_LineCapMode, element.settings.lineCapMode, DEFAULT_LINE_CAP_MODE);
	SETTINGV(element.functionVariant <= F9, CP_Settings_LineJointMode, element.settings.lineJointMode, DEFAULT_LINE_JOINT_MODE);
	SETTINGV(RANGE(TXT, element.functionVariant, TXT_B), CP_Font_Set, element.settings.textFont, CP_Font_GetDefault());
	SETTINGV(RANGE(TXT, element.functionVariant, TXT_B), CP_Settings_TextSize, element.settings.textSize, DEFAULT_TEXT_SIZE);
	// Align has 2 parameters so I do it manually since its the only situation for it so no reason to make a macro
	if (TXT <= element.functionVariant && element.functionVariant <= TXT_B) {
		CP_TEXT_ALIGN_HORIZONTAL alignH = NULL_COALESCING(element.settings.textAlignH, DEFAULT_TEXT_ALIGN_H);
		CP_TEXT_ALIGN_VERTICAL alignV = NULL_COALESCING(element.settings.textAlignV, DEFAULT_TEXT_ALIGN_V);
		CP_Settings_TextAlignment(alignH, alignV);
	}
	else { assert(!(element.settings.textAlignH || element.settings.textAlignV)); }
	
	switch (element.functionVariant) { // this is also horrendous
		case F2:
			element.draw.f2(ELE(0, f), ELE(1, f));
			break;
		case F3:
			element.draw.f3(ELE(0, f), ELE(1, f), ELE(2, f));
			break;
		case F4:
			element.draw.f4(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f));
			break;
		case F5:
			element.draw.f5(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f));
			break;
		case F6:
			element.draw.f6(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f));
			break;
		case F7:
			element.draw.f7(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f), ELE(6, f));
			break;
		case F8:
			element.draw.f8(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f), ELE(6, f), ELE(7, f));
			break;
		case F9:
			element.draw.f9(ELE(0, f), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f), ELE(6, f), ELE(7, f), ELE(8, f));
			break;
		case TXT:
			element.draw.txt(CONST_ELE(0, text), ELE(1, f), ELE(2, f));
			break;
		case TXT_B:
			element.draw.txtB(CONST_ELE(0, text), ELE(1, f), ELE(2, f), ELE(3, f));
			break;
		case IMG:
			element.draw.img(CONST_ELE(0, image), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, i));
			break;
		case IMG_A:
			element.draw.imgA(CONST_ELE(0, image), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, i), ELE(6, f));
			break;
		case IMG_S:
			element.draw.imgS(CONST_ELE(0, image), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f), ELE(6, f), ELE(7, f), ELE(8, f), ELE(9, i));
			break;
		case IMG_SA:
			element.draw.imgSA(CONST_ELE(0, image), ELE(1, f), ELE(2, f), ELE(3, f), ELE(4, f), ELE(5, f), ELE(6, f), ELE(7, f), ELE(8, f), ELE(9, i), ELE(10, f));
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