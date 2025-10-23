#pragma once

#include <stdlib.h>

typedef struct DynamicVariable {
    void* value;
    struct DynamicVariable* next;
} DynamicVariable;

void* CreateDynamicVariable(size_t size);
CP_Image CreateDynamicImage(const char* filepath);
void FreeAllDynamicVariables(void);