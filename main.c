#include<stdio.h>
#include<raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



int main() {
    // Inicializar la ventana
    InitWindow(625, 600, "Ejemplo de pestañas con raygui");

    // Variables para el control de pestañas
    int currentTab = 0;
    const char *tabNames = "Carga de artículos;Consulta por código;Listado Completo";
    char InputNombreArticulo[50] = {0};
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        
        // Aca ba la logica de la actualización antes de dibujar 
        //
        //
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar los botones de las pestañas usando GuiToggleGroup
        GuiToggleGroup((Rectangle){10, 10, 200, 40}, tabNames, &currentTab);

        // Contenido de cada pestaña
        if (currentTab == 0) {
            GuiGroupBox((Rectangle){10, 60, 415, 80},"Artículo");
            GuiLabel((Rectangle){50, 80, 200, 40},"Descripción");
            GuiTextBox((Rectangle){210, 80, 200, 40},InputNombreArticulo, 50,false);
        } else if (currentTab == 1) {
            GuiGroupBox((Rectangle){10, 60, 200, 80},"Artículo");
        } else if (currentTab == 2) {
            GuiGroupBox((Rectangle){10, 60, 200, 80},"Listado");
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;

}