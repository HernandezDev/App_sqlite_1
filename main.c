#include<stdio.h>
#include<raylib.h>
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

struct Articulo
{
    int Id;
    char IdStr[50];
    char Nombre[50];
    float Precio;
    char PrecioStr[50];
};


int main() {
    // Inicializar la ventana
    InitWindow(625, 400, "App Sqlite Raylib");

    // Variables para el control de pestañas
    int currentTab = 0;
    const char *tabNames = "Carga de artículos;Consulta por Id;Listado Completo";

    // Declarar struts para trasladar datos  (input) (consulta)
    struct Articulo input = {0,"","", 0.0f, ""};
    struct Articulo consulta = {0,"","", 0.0f, ""};

    bool EditInputNombreArticulo = false;
    bool EditInputPrecioArticulo = false;
    bool EditConsultaId = false;
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
                EditConsultaId = false;
            }
            // Verificar si se hizo clic en el campo de texto del precio del artículo
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 130, 200, 40}) && currentTab == 0)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = true;
                EditConsultaId = false;
            } 
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 80, 200, 40}) && currentTab == 1)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = true;
            }
            else 
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = false;
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
            //nombre
            GuiLabel((Rectangle){25, 80, 200, 40}, "Nombre");
            GuiTextBox((Rectangle){90, 80, 200, 40}, input.Nombre, 50, EditInputNombreArticulo);

            // precio
            GuiLabel((Rectangle){25, 130, 200, 40}, "Precio");
            GuiValueBoxFloat((Rectangle){90, 130, 200, 40}, "", input.PrecioStr, &input.Precio, EditInputPrecioArticulo);
            //confirmar
            if (GuiButton((Rectangle){90, 180, 200, 40}, "Confirmar")) 
            {
                // Aquí puedes agregar la lógica para manejar el botón "Confirmar"
                
            }
        }
        else if (currentTab == 1) 
        {
            GuiGroupBox((Rectangle){10, 60, 300, 240}, "Artículo");
            //Id
            GuiLabel((Rectangle){25, 80, 200, 40}, "Id");
            GuiValueBox((Rectangle){90, 80, 200, 40}, consulta.IdStr,&consulta.Id,0,10000000,EditConsultaId);
            //nombre
            GuiLabel((Rectangle){25, 130, 200, 40}, "Nombre");
            GuiTextBox((Rectangle){90, 130, 200, 40}, consulta.Nombre, 50, false);
            //precio
            GuiLabel((Rectangle){25, 180, 200, 40}, "Precio");
            GuiValueBoxFloat((Rectangle){90, 180, 200, 40}, "", consulta.PrecioStr, &consulta.Precio, false);
            //Consultar
            if (GuiButton((Rectangle){90, 230, 200, 40}, "Consultar")) 
            {
                // Aquí puedes agregar la lógica para manejar el botón "Confirmar"
                
            }
        }
        else if (currentTab == 2) 
        {
            GuiGroupBox((Rectangle){10, 60, 605, 330}, "Listado");
            
        }

        EndDrawing();
    }
    CloseWindow();
    return 0;
}