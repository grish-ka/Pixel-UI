#include "raylib.h"
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 

// --- RAYGUI COMPATIBILITY FIX ---
float TextToFloat(const char *text) {
    return (float)atof(text);
}

#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include <pixel_ui.h> 

// Define grid dimensions based on screen size (800x440) and pixel size (40)
#define GRID_W 20
#define GRID_H 11

int main(void) {
    const int screenWidth = 800;  
    const int screenHeight = 440; 
    
    // --- COLOR PICKER WINDOW PARAMETERS ---
    #define CUSTOM_COLOR_INDEX 3 
    #define ERASE_INDEX 4 
    Rectangle colorPickerRec = { 140, 10, 200, 260 }; // Increased height for the button

    InitWindow(screenWidth, screenHeight, "Pixel UI Shared Lib Test");
    SetTargetFPS(60);

    GuiSetFont(GetFontDefault());

    // --- Application State ---
    int selectedColor = 0; // 0=RED, 1=GREEN, 2=BLUE, 3=CUSTOM, 4=ERASE
    bool dropdownEditMode = false; 
    bool showColorPicker = false; 
    
    Color customColor = WHITE; 
    
    Color colors[5] = { RED, GREEN, BLUE, WHITE, BLANK }; 

    Color paintColor; 
    
    // Flag to track if a UI control was clicked this frame
    bool uiControlClicked = false; 

    // This array will store the color of every pixel on our screen
    Color paintGrid[GRID_W][GRID_H];
    for(int x=0; x<GRID_W; x++) {
        for(int y=0; y<GRID_H; y++) {
            paintGrid[x][y] = BLANK;
        }
    }

    TraceLog(LOG_INFO, "TEST: Entering Main Loop...");

    while (!WindowShouldClose()) {
        
        // --- Logic ---
        int mouseGridX = GetMouseX() / PIXEL_SIZE;
        int mouseGridY = GetMouseY() / PIXEL_SIZE;
        Vector2 mousePos = GetMousePosition();

        // 1. UPDATE CUSTOM COLOR IN ARRAY
        colors[CUSTOM_COLOR_INDEX] = customColor;

        // 2. GET CURRENT PAINT COLOR
        paintColor = colors[selectedColor];

        // 3. HANDLE MODE SWITCHING AND CURSOR
        const char* current_mode = (selectedColor == ERASE_INDEX) ? "ERASE" : "PAINT"; 

        if (dropdownEditMode || showColorPicker) {
            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
        } else if (selectedColor == ERASE_INDEX) {
            SetMouseCursor(MOUSE_CURSOR_NOT_ALLOWED); 
        } else {
            SetMouseCursor(MOUSE_CURSOR_CROSSHAIR); 
        }
        
        // 4. PAINTING/ERASING LOGIC (Left Click - Applies selected tool)
        // FIX: Added uiControlClicked check to prevent painting when clicking UI
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !dropdownEditMode && !showColorPicker && !uiControlClicked) {
            if (mouseGridX >= 0 && mouseGridX < GRID_W &&
                mouseGridY >= 0 && mouseGridY < GRID_H) {
                
                paintGrid[mouseGridX][mouseGridY] = paintColor;
            }
        }
        
        // Right Mouse Button Utility Erase
        if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && !dropdownEditMode && !showColorPicker && !uiControlClicked) {
            if (mouseGridX >= 0 && mouseGridX < GRID_W &&
                mouseGridY >= 0 && mouseGridY < GRID_H) {
                
                paintGrid[mouseGridX][mouseGridY] = BLANK;
            }
        }
    
        // --- Drawing ---
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            uiControlClicked = false; // Reset the click flag at the start of drawing

            // 1. Draw Grid Lines
            for (int x = 0; x <= screenWidth; x += PIXEL_SIZE) {
                DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
            }
            for (int y = 0; y <= screenHeight; y += PIXEL_SIZE) {
                DrawLine(0, y, screenWidth, y, LIGHTGRAY);
            }

            // 2. Draw all the painted pixels
            for(int x=0; x<GRID_W; x++) {
                for(int y=0; y<GRID_H; y++) {
                    if (paintGrid[x][y].a != 0) {
                        SetPixel(x, y, paintGrid[x][y]);
                    }
                }
            }

            // 3. Draw Hover Effect (Only draw if no picker is open)
            bool isMouseOverGrid = CheckCollisionPointRec(mousePos, (Rectangle){0, 0, screenWidth, screenHeight});
            if (isMouseOverGrid && !dropdownEditMode && !showColorPicker) {
                
                Color hoverColor;
                
                if (selectedColor == ERASE_INDEX) {
                    hoverColor = Fade(BLANK, 0.5f); 
                } else {
                    hoverColor = Fade(paintColor, 0.3f); 
                }
                
                SetPixel(mouseGridX, mouseGridY, hoverColor); 
            }

            // --- UI (View) ---
            
            // Temporary variable to hold the color status string
            const char* colorStatus;
            if (selectedColor == CUSTOM_COLOR_INDEX) {
                colorStatus = "CUSTOM";
            } else if (selectedColor == ERASE_INDEX) {
                colorStatus = "ERASE";
            } else {
                colorStatus = "PRESET";
            }

            // Info Label
            GuiLabel((Rectangle){ 10, screenHeight - 30, 400, 20 }, 
                TextFormat("Mode: %s | Color: %s | Mouse: (%d, %d).", 
                current_mode, 
                colorStatus,  
                mouseGridX,   
                mouseGridY)); 

            // Dropdown Menu: List now includes "CUSTOM" and "ERASE"
            if (GuiDropdownBox((Rectangle){ 10, 10, 150, 30 }, "RED;GREEN;BLUE;CUSTOM;ERASE", &selectedColor, dropdownEditMode)) {
                dropdownEditMode = !dropdownEditMode; 
                uiControlClicked = true; // Mark as clicked when dropdown state changes
            }

            // --- COLOR PICKER LOGIC ---
            if (selectedColor == CUSTOM_COLOR_INDEX) {
                showColorPicker = true;
            } else {
                // Check if the user is clicking the close button on the window/menu
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                    showColorPicker = false; // Hide the picker if custom is deselected
                }
            }

            // Draw the actual Color Picker UI
            if (showColorPicker) {
                // GuiWindowBox allows dragging but doesn't handle the close button state
                if (GuiWindowBox(colorPickerRec, "Custom Color Picker")) {
                    // This specifically runs if the close button in GuiWindowBox is clicked
                    showColorPicker = false;
                    selectedColor = 0; // Revert to a safe preset (RED)
                    uiControlClicked = true; 
                }
                
                // CRITICAL raygui function: GuiColorPicker
                GuiColorPicker((Rectangle){ colorPickerRec.x + 10, colorPickerRec.y + 30, colorPickerRec.width - 20, colorPickerRec.height - 70 }, // Adjusted height
                                 "Color", 
                                 &customColor);
                                 
                // New Close Button inside the picker window
                if (GuiButton((Rectangle){ colorPickerRec.x + 10, colorPickerRec.y + colorPickerRec.height - 35, colorPickerRec.width - 20, 25 }, "CLOSE")) {
                    showColorPicker = false;
                    selectedColor = 0; // Revert to a safe preset (RED)
                    uiControlClicked = true;
                }
            }
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}