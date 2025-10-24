#pragma once

enum FunctionVariant { F2, F3, F4, F5, F6, F7, F8, F9, TXT, TXT_B, IMG, IMG_A, IMG_S, IMG_SA };
typedef struct UIElement UIElement;

typedef struct DrawSettings {
	CP_Color drawColor;
	CP_Color strokeColor;
	float strokeWeight;
	CP_LINE_CAP_MODE lineCapMode;
	CP_LINE_JOINT_MODE lineJointMode;
	CP_Font textFont;
	float textSize;
	CP_TEXT_ALIGN_HORIZONTAL textAlignH;
	CP_TEXT_ALIGN_VERTICAL textAlignV;
} DrawSettings;
typedef union DrawFunction { // Well this is horrendous
	void (*f2)(float, float);
	void (*f3)(float, float, float);
	void (*f4)(float, float, float, float);
	void (*f5)(float, float, float, float, float);
	void (*f6)(float, float, float, float, float, float);
	void (*f7)(float, float, float, float, float, float, float);
	void (*f8)(float, float, float, float, float, float, float, float);
	void (*f9)(float, float, float, float, float, float, float, float, float);
	void (*txt)(const char* text, float x, float y);
	void (*txtB)(const char* text, float x, float y, float rowWidth);
	void (*img)(CP_Image img, float x, float y, float w, float h, int alpha);
	void (*imgA)(CP_Image img, float x, float y, float w, float h, int alpha, float degrees);
	void (*imgS)(CP_Image img, float x, float y, float w, float h, float u0, float v0, float u1, float v1, int alpha);
	void (*imgSA)(CP_Image img, float x, float y, float w, float h, float u0, float v0, float u1, float v1, int alpha, float degrees);
} DrawFunction;
typedef union DrawParam { // float is first in both as its most used so this minimizes explicitely saying which type I want from the union
	float f;
	int i;
	char* text;
	CP_Image image;
} DrawParam;
typedef union DrawParamChange {
	float (*f)(UIElement _this); // if an object has any dynamic movement, it'll be constant, based off the parent, or based off sibling index. UIElement has all 3
	int (*i)(UIElement _this);
	void (*none)(UIElement _this);
} DrawParamChange;
struct UIElement {
	// core data
	int currentlyEnabled;
	DrawSettings settings;
	enum FunctionVariant functionVariant;
	union DrawFunction draw;
	union DrawParam initialParameters[11];
	union DrawParamChange parameterChanges[11];
	
	// related UI elements
	int childCount;
	int siblingIndex;
	struct UIElement* parent;
	struct UIElement* firstChild;
	struct UIElement* nextSibling;
};
static int Int0(UIElement trashVar) { return 0; }
static float Float0(UIElement trashVar) { return 0.F; }
static void NO_INPUT(UIElement trashVar) { }

UIElement* GetLastChild(UIElement element);
void DrawUIElement(UIElement element);
UIElement* CreateUIElement(UIElement* parent, DrawSettings settings, union DrawFunction draw, enum FunctionVariant functionVariant, int paramCount, union DrawParam initialParameters[11], union DrawParamChange parameterChanges[11]);