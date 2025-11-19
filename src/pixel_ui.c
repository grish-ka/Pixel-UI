#include "pixel_ui.h"

// We removed the 'const int PIXEL_SIZE' variable because we are using a #define now.

Vector2 GetPixelScreenPos(int gridX, int gridY) {
    Vector2 screenPos;
    screenPos.x = (float)(gridX * PIXEL_SIZE);
    screenPos.y = (float)(gridY * PIXEL_SIZE);
    return screenPos;
}

void SetPixel(int gridX, int gridY, Color color) {
    Vector2 pos = GetPixelScreenPos(gridX, gridY);
    DrawRectangle((int)pos.x, (int)pos.y, PIXEL_SIZE, PIXEL_SIZE, color);
}