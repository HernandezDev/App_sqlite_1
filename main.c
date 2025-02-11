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

    // Define el rango de caracteres que necesitas (puedes ajustar este rango según tus necesidades)
    int fontSize = 20; // Tamaño de la fuente
    int charsCount = 256; // Número de caracteres a cargar

    // Carga la fuente DejaVu Sans con un rango de caracteres específico
    Font font = LoadFontEx("C:\\Windows\\Fonts\\DejaVuSans.ttf", fontSize, 0, charsCount);
    GuiSetFont(font);

    bool showMessageBox = false;

    while (!WindowShouldClose())
    {
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
            ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

            // Muestra un texto de prueba con caracteres especiales
            DrawTextEx(font, "Texto de prueba: ñ á é í ó ú ü", (Vector2){20, 20}, fontSize, 2, BLACK);

            
        EndDrawing();
    }

    // De-Initialization
    UnloadFont(font);  // Descartar la fuente cargada
    CloseWindow();  // Close window and OpenGL context

    return 0;
}