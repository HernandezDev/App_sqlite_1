#include<stdio.h>
#include<raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



int main() {
    // Inicializar la ventana
    InitWindow(625, 600, "Ejemplo de pestañas con raygui");

    // Variables para el control de pestañas
    int currentTab = 0;
    const char *tabNames = "Tab1;Tab2;Tab3";

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar los botones de las pestañas usando GuiToggleGroup
        GuiToggleGroup((Rectangle){10, 10, 200, 40}, tabNames, &currentTab);

        // Contenido de cada pestaña
        if (currentTab == 0) {
            DrawText("Contenido de Tab1", 20, 60, 20, DARKGRAY);
        } else if (currentTab == 1) {
            DrawText("Contenido de Tab2", 20, 60, 20, DARKGRAY);
        } else if (currentTab == 2) {
            DrawText("Contenido de Tab3", 20, 60, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;

}