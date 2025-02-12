#include<stdio.h>
#include<raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

struct Articulo
{
    int Id;
    char Descrp[50];
    float Precio;
    char PrecioStr[50];
};


int main() {
    // Inicializar la ventana
    InitWindow(625, 400, "App Sqlite Raylib");

    // Variables para el control de pestañas
    int currentTab = 0;
    const char *tabNames = "Carga de artículos;Consulta por código;Listado Completo";

    // Declarar struts para trasladar datos  (input) (consulta)
    struct Articulo input = {0, "", 0.0f, ""};
    struct Articulo consulta = {0, "", 0.0f, ""};

    bool EditInputNombreArticulo = false;
    bool EditInputPrecioArticulo = false;
    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        // Lógica de la actualización antes de dibujar 
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) 
        {
            Vector2 mousePoint = GetMousePosition();

            // Verificar si se hizo clic en el campo de texto del nombre del artículo
            if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 80, 200, 40}) && currentTab == 0) 
            {
                EditInputNombreArticulo = true;
                EditInputPrecioArticulo = false;
            }
            // Verificar si se hizo clic en el campo de texto del precio del artículo
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 130, 200, 40}) && currentTab == 0)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = true;
            } 
            else 
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar los botones de las pestañas usando GuiToggleGroup
        GuiToggleGroup((Rectangle){10, 10, 200, 40}, tabNames, &currentTab);

        // Contenido de cada pestaña
        if (currentTab == 0) 
        {
            GuiGroupBox((Rectangle){10, 60, 300, 200}, "Artículo");
            GuiLabel((Rectangle){25, 80, 200, 40}, "Descripción");
            GuiTextBox((Rectangle){90, 80, 200, 40}, input.Descrp, 50, EditInputNombreArticulo);

            // Usar GuiValueBoxFloat para manejar valores flotantes
            GuiValueBoxFloat((Rectangle){90, 130, 200, 40}, "Precio ", input.PrecioStr, &input.Precio, EditInputPrecioArticulo);
            if (GuiButton((Rectangle){90, 180, 200, 40}, "Confirmar")) 
            {
                // Aquí puedes agregar la lógica para manejar el botón "Confirmar"
                
            }
        }
        else if (currentTab == 1) 
        {
            GuiGroupBox((Rectangle){10, 60, 200, 80}, "Artículo");
        }
        else if (currentTab == 2) 
        {
            GuiGroupBox((Rectangle){10, 60, 200, 80}, "Listado");
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}