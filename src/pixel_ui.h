#ifndef PIXEL_UI_H
#define PIXEL_UI_H

#include "raylib.h"

// --- Windows DLL Export Magic ---
#if defined(_WIN32)
    #if defined(BUILDING_PIXEL_UI)
        #define PIXEL_UI_API __declspec(dllexport)
    #else
        #define PIXEL_UI_API __declspec(dllimport)
    #endif
#else
    #define PIXEL_UI_API
#endif

// --- Public Constants ---
// FIX: Use #define instead of extern variable to prevent Div-By-Zero crashes
#define PIXEL_SIZE 40

// --- Public Functions ---
PIXEL_UI_API Vector2 GetPixelScreenPos(int gridX, int gridY);
PIXEL_UI_API void SetPixel(int gridX, int gridY, Color color);

#endif // PIXEL_UI_H