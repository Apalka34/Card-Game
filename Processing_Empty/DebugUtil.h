#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>

#define BUFFER_SIZE 256

static void DebugPrintf(const char* string, ...)
{
    char buffer[BUFFER_SIZE] = { 0 };

    va_list args;
    va_start(args, string);

    // Safe version in MSVC: _vsnprintf_s
    _vsnprintf_s(buffer, BUFFER_SIZE, _TRUNCATE, string, args);
    va_end(args);

    OutputDebugStringA(buffer);
}