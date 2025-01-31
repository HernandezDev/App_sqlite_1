#include<stdio.h>
#include<raylib.h>

int main(void)
{
   // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Ejemplo de ventana con raylib");

    SetTargetFPS(60);  // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose())  // Detect window close button or ESC key
    {
        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hola soy una ventana re fachera!", 190, 200, 20, BLACK);

        EndDrawing();
    }

    // De-Initialization
    CloseWindow();  // Close window and OpenGL context
}