#pragma once

#include <stdlib.h>

typedef struct DynamicVariable {
    void* value;
    struct DynamicVariable* next;
} DynamicVariable;

void* CreateDynamicVariable(size_t size);
CP_Image CreateDynamicImage(const char* filepath);
CP_Font CreateDynamicFont(const char* filepath);
CP_Sound CreateDynamicSound(const char* filepath);
void FreeAllDynamicVariables(void);