#include <stdlib.h>
#include "cprocessing.h"
#include "DebugUtil.h"
#include "DynamicMemoryManager.h"

DynamicVariable* firstDynamicVariable = NULL;
DynamicVariable* lastDynamicVariable = NULL;

/// <summary>
/// Makes a piece of data be a dynamic variable that can be easily freed upon exiting the game
/// </summary>
/// <param name="data">The data to be stored</param>
/// <param name="size">The size memory allocation of data</param>
/// <returns>NULL if memory allocation failed. data if memory allocation succeeded</returns>
void* CreateDynamicVariable(size_t size) {
    void* data = malloc(size);
    if (data == NULL) return NULL;

    DynamicVariable* node = malloc(sizeof(DynamicVariable));
    if (node == NULL) {
        free(data);
        return NULL;
    }
    node->value = data;
    node->next = NULL;
    if (firstDynamicVariable == NULL) {
        firstDynamicVariable = node;
        lastDynamicVariable = node;
    }
    else {
        lastDynamicVariable->next = node;
        lastDynamicVariable = node;
    }

    DebugPrintf("reserved memory using node %p for variable %p\n", node, node->value);

    return data;
}
CP_Image CreateDynamicImage(const char* filepath) {
    CP_Image image = CP_Image_Load(filepath);
    DynamicVariable* var = CreateDynamicVariable(sizeof(CP_Image));
    var->value = image;
    return image;
}
CP_Font CreateDynamicFont(const char* filepath) {
    CP_Font font = CP_Font_Load(filepath);
    DynamicVariable* var = CreateDynamicVariable(sizeof(CP_Font));
    var->value = font;
    return font;
}
CP_Sound CreateDynamicSound(const char* filepath) {
    CP_Sound sound = CP_Sound_Load(filepath);
    DynamicVariable* var = CreateDynamicVariable(sizeof(CP_Sound));
    var->value = sound;
    return sound;
}
void FreeAllDynamicVariables(void) {
    DynamicVariable* node = firstDynamicVariable;
    while (node) {
        DynamicVariable* next = node->next;
        DebugPrintf("freeing memory of node %p and its variable %p\n", node, node->value);
        free(node->value);
        free(node);
        node = next;
    }
    firstDynamicVariable = NULL;
    lastDynamicVariable = NULL;
}