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

            // Muestra un texto de prueba con caracteres especiales
            
            DrawText("Texto de prueba: ñ á é í ó ú ü", 20, 50, 20, BLACK);

            
        EndDrawing();
    }

    // De-Initialization
    
    CloseWindow();  // Close window and OpenGL context

    return 0;
}