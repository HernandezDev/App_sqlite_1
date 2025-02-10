#include<stdio.h>
#include<raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

int main(void)
{
   // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Ejemplo de ventana con raylib");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second
    bool showMessageBox = false;

    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            if (GuiButton((Rectangle){ 24, 24, 120, 30 }, "#191#Show Message")) showMessageBox = true;

            if (showMessageBox)
            {
                int result = GuiMessageBox((Rectangle){ 85, 70, 250, 100 },
                    "#191#Message Box", "Hi! This is a message!", "Nice;Cool");

                if (result >= 0) showMessageBox = false;
            }

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context
}