#include<stdio.h>
#include<sqlite3.h>
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

bool SQL_AbrirBase(sqlite3 **db, const char *nombre_db) 
{
    int rc = sqlite3_open(nombre_db, db);
    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "No se puede abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        sqlite3_close(*db);
        return false; // Indica que hubo un error
    }
    return true; // Indica que la base de datos se abrió correctamente
}

bool SQL_EjecutarSentencia(sqlite3 *db, const char *sql, const char *operacion) 
{
    char *err_msg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "Error de SQL (%s): %s\n", operacion, err_msg);
        sqlite3_free(err_msg);
        return false; // Indica que hubo un error
    }
    return true; // Indica que la operación fue exitosa
}

bool SQL_PrepararSentencia(sqlite3 *db, sqlite3_stmt **stmt, const char *sql) 
{
    int rc = sqlite3_prepare_v2(db, sql, -1, stmt, 0);
    if (rc != SQLITE_OK) 
    {
        fprintf(stderr, "No se pudo preparar la sentencia: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(*stmt); // Finalizar la sentencia en caso de error
        return false; // Indicar que hubo un error
    }
    return true; // Indicar que la preparación fue exitosa
}

bool InciarBase(sqlite3 *db)
{
    char *sql_create;       //crear tabla
    char *sql_index;        //crear el indice para no repetir el nombre
    char *sql_trigger_INSERT_nombre; //trigger para no insertan nombre vacio
    char *sql_trigger_UPDATE_nombre; //trigger para no actualizar nombre vacio
    char *sql_trigger_INSERT_precio; //trigger para no insertar cero o negativo
    char *sql_trigger_UPDATE_precio; //trigger para no actualizar cero o negativo

    //crear la tabla
    sql_create = "CREATE TABLE IF NOT EXISTS Articulos(Id INTEGER PRIMARY KEY AUTOINCREMENT, Nombre TEXT, Precio REAL);";
    if (!SQL_EjecutarSentencia(db,sql_create,"Crear Tabla"))
    {
        return false;
    }
    // crear indice para no repetir Nombre 
    sql_index = "CREATE UNIQUE INDEX IF NOT EXISTS idx_nombre_unico ON Articulos(Nombre);";
    if (!SQL_EjecutarSentencia(db,sql_index,"Crear Indice"))
    {
        return false;
    }
    //trigger para no insertar nombre vacio
    sql_trigger_INSERT_nombre = "CREATE TRIGGER IF NOT EXISTS Insert_Nombre"
                                " BEFORE INSERT ON Articulos"
                                " FOR EACH ROW"
                                " BEGIN"
                                " SELECT CASE WHEN NEW.Nombre = '' THEN"
                                " RAISE(ABORT, 'Nombre no puede estar vacio')"
                                " END;"
                                " END;";

    if (!SQL_EjecutarSentencia(db,sql_trigger_INSERT_nombre,"Crear Trigger Insert"))
    {
        return false;
    }
    //trigger para no actualizar nombre vacio
    sql_trigger_UPDATE_nombre = "CREATE TRIGGER IF NOT EXISTS Update_Nombre"
                                " BEFORE UPDATE OF Nombre ON Articulos"
                                " FOR EACH ROW"
                                " BEGIN"
                                " SELECT CASE WHEN NEW.Nombre = '' THEN"
                                " RAISE(ABORT, 'Nombre no puede estar vacio')"
                                " END;"
                                " END;";
    if (!SQL_EjecutarSentencia(db,sql_trigger_UPDATE_nombre,"Crear Trigger Update"))
    {
        return false;
    }
    //trigger para no insertar precio cero o negativo
    sql_trigger_INSERT_precio = "CREATE TRIGGER IF NOT EXISTS Insert_Precio"
                                " BEFORE INSERT ON Articulos"
                                " FOR EACH ROW"
                                " BEGIN"
                                " SELECT CASE WHEN NEW.Precio <= 0 THEN"
                                " RAISE(ABORT, 'Precio no puede ser cero o negativo')"
                                " END;"
                                " END;";
    if (!SQL_EjecutarSentencia(db,sql_trigger_INSERT_precio,"Crear Trigger Insert Precio"))
    {
        return false;
    }
    //trigger para no actualizar precio cero o negativo
    sql_trigger_UPDATE_precio = "CREATE TRIGGER IF NOT EXISTS Update_Precio"
                                " BEFORE UPDATE OF Precio ON Articulos"
                                " FOR EACH ROW"
                                " BEGIN"
                                " SELECT CASE WHEN NEW.Precio <= 0 THEN"
                                " RAISE(ABORT, 'Precio no puede ser cero o negativo')"
                                " END;"
                                " END;";
    if (!SQL_EjecutarSentencia(db,sql_trigger_UPDATE_precio,"Crear Trigger Update Precio"))
    {
        return false;
    }
    return true;
}

int CargarAriculo(sqlite3 *db, struct Articulo *input)
{
    sqlite3_stmt *stmt;
    char *sql_insert;
    sql_insert="INSERT INTO Articulos(Nombre, Precio) VALUES(?, ?);";
    if (!SQL_PrepararSentencia(db,&stmt,sql_insert))
    {
        return 2;
    }
    sqlite3_bind_text(stmt, 1, input->Nombre, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, input->Precio);
    if (sqlite3_step(stmt)!=SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1;
    }
    else
    {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int ConsultarAriculo(sqlite3 *db, struct Articulo *consulta)
{
    sqlite3_stmt *stmt;
    char *sql_select;
    sql_select = "SELECT Id, Nombre, Precio FROM Articulos WHERE Id = ?;";
    if (!SQL_PrepararSentencia(db,&stmt,sql_select))
    {
        return 2;
    }
    sqlite3_bind_int(stmt,1,consulta->Id);
    if (sqlite3_step(stmt)==SQLITE_ROW)
    {
        strcpy(consulta->Nombre, (const char*)sqlite3_column_text(stmt, 1));
        consulta->Precio=sqlite3_column_double(stmt, 2);
        sqlite3_finalize(stmt);
        return 0;
    }
    else
    {
        sqlite3_finalize(stmt);
        return 1;
    }
}

int EditarArticulo(sqlite3 *db, struct Articulo *consulta)
{
    sqlite3_stmt *stmt;
    char *sql_update;
    sql_update="UPDATE Articulos SET Nombre = ?, Precio = ? WHERE Id = ?;";
    if (!SQL_PrepararSentencia(db,&stmt,sql_update))
    {
        return 2;
    }
    sqlite3_bind_text(stmt, 1, consulta->Nombre, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 2, consulta->Precio);
    sqlite3_bind_int(stmt, 3, consulta->Id);
    if (sqlite3_step(stmt)!=SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1;
    }
    else
    {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int BorrarArticulo(sqlite3 *db, struct Articulo *consulta)
{
    sqlite3_stmt *stmt;
    char *sql_delete;
    sql_delete="DELETE FROM Articulos WHERE Id = ?;";
    if (!SQL_PrepararSentencia(db,&stmt,sql_delete))
    {
        return 2;
    }
    sqlite3_bind_int(stmt,1,consulta->Id);
    if (sqlite3_step(stmt)!=SQLITE_DONE)
    {
        sqlite3_finalize(stmt);
        return 1;
    }
    else
    {
        sqlite3_finalize(stmt);
        return 0;
    }
}

int CantidadRows(sqlite3 *db)
{
    int cantidad=0;
    sqlite3_stmt *stmt;
    char *sql_select;
    sql_select="SELECT COUNT(*) FROM Articulos;";
    SQL_PrepararSentencia(db,&stmt,sql_select);
    if (sqlite3_step(stmt)==SQLITE_ROW)
    {
        cantidad = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);
    return cantidad;
}



bool CargarLista(sqlite3 *db, struct Articulo *Lista)
{
    int i=0;
    sqlite3_stmt *stmt;
    char *sql_select;
    sql_select="SELECT Id, Nombre, Precio FROM Articulos;";
    if (!SQL_PrepararSentencia(db,&stmt,sql_select))
    {
        return false;
    }
    while (sqlite3_step(stmt)==SQLITE_ROW)
    {
        Lista[i].Id=sqlite3_column_int(stmt, 0);
        sprintf(Lista[i].IdStr,"%d",Lista[i].Id);
        strcpy(Lista[i].Nombre, (const char*)sqlite3_column_text(stmt, 1));
        Lista[i].Precio=sqlite3_column_double(stmt, 2);
        sprintf(Lista[i].PrecioStr,"%.2f",Lista[i].Precio);
        i++;
    }
    sqlite3_finalize(stmt);
    return true;
}


int main() {
    sqlite3 *db;
    if (!SQL_AbrirBase(&db,"Base.db"))
    {
        return 1;
    }
    if (!InciarBase(db))
    {
        return 1;
    }
    
    
    // Inicializar la ventana
    InitWindow(625, 400, "ArticleDB");

    // Cargar el icono de la ventana
    Image icon = LoadImage("icon.png");  // Cargar la imagen del icono
    SetWindowIcon(icon);                 // Establecer el icono de la ventana

    // Variables para el control de pestañas
    int currentTab = 0;
    const char *tabNames = "Carga de artículos;Consulta por Id;Listado Completo";
    bool editMode = false;
    bool consulatdo = false;
    
    // Declarar struts para trasladar datos  (input) (consulta)
    struct Articulo input = {0,"","", 0.0f, ""};
    struct Articulo consulta = {0,"","", 0.0f, ""};

    //ventana de mensaje confirmar
    bool MensajeActivo=false;
    char titulo[50]="";
    char mensaje[50]="";
    //boxes editables
    bool EditInputNombreArticulo = false;
    bool EditInputPrecioArticulo = false;
    bool EditConsultaId = false;
    bool EditConsultaNombre = false;
    bool EditConsultaPrecio = false;

    //Arreglo de lista
    int LargoLista=CantidadRows(db);
    struct Articulo *Lista=malloc(LargoLista * sizeof(struct Articulo));
    CargarLista(db, Lista);

    // Variables para el desplazamiento
    Vector2 scroll = {0, 60};
    float alturaFila = 30; // Altura de cada fila
    float espacioFilas = 10; // Espacio entre filas

    SetTargetFPS(60);

    while (!WindowShouldClose()) 
    {
        // Lógica de la actualización antes de dibujar 
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&!MensajeActivo) 
        {
            Vector2 mousePoint = GetMousePosition();

            // Verificar si se hizo clic en el campo de texto del nombre del artículo
            if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 80, 200, 40}) && currentTab == 0) 
            {
                EditInputNombreArticulo = true;
                EditInputPrecioArticulo = false;
                EditConsultaId = false;
                EditConsultaNombre = false;
                EditConsultaPrecio = false;
            }
            // Verificar si se hizo clic en el campo de texto del precio del artículo
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 130, 200, 40}) && currentTab == 0)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = true;
                EditConsultaId = false;
                EditConsultaNombre = false;
                EditConsultaPrecio = false;
            } 
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 80, 200, 40}) && currentTab == 1 && !editMode)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = true;
                EditConsultaNombre = false;
                EditConsultaPrecio = false;
            }
            
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 130, 200, 40}) && currentTab == 1 && editMode)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = false;
                EditConsultaNombre = true;
                EditConsultaPrecio = false;
            }
            else if (CheckCollisionPointRec(mousePoint, (Rectangle){90, 180, 200, 40}) && currentTab == 1 && editMode)
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = false;
                EditConsultaNombre = false;
                EditConsultaPrecio = true;
            }
            else 
            {
                EditInputNombreArticulo = false;
                EditInputPrecioArticulo = false;
                EditConsultaId = false;
                EditConsultaNombre = false;
                EditConsultaPrecio = false;

            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar los botones de las pestañas usando GuiToggleGroup
        GuiToggleGroup((Rectangle){10, 10, 200, 40}, tabNames, &currentTab);

        // Contenido de cada pestaña
        if (currentTab == 0 ) 
        {
            
            GuiGroupBox((Rectangle){10, 60, 300, 200}, "Artículo");
            //nombre
            GuiLabel((Rectangle){25, 80, 200, 40}, "Nombre");
            GuiTextBox((Rectangle){90, 80, 200, 40}, input.Nombre, 50, EditInputNombreArticulo);

            // precio
            GuiLabel((Rectangle){25, 130, 200, 40}, "Precio");
            GuiValueBoxFloat((Rectangle){90, 130, 200, 40}, "", input.PrecioStr, &input.Precio, EditInputPrecioArticulo);
            //confirmar
            if (GuiButton((Rectangle){90, 180, 200, 40}, "Confirmar") && !MensajeActivo) 
            {
                
                MensajeActivo=true;
                //progrmar mensajes con exito errores y reiniciar  el struct
                // 0 exito / 1 nombre repetido / 2 error en la base de datos
                switch (CargarAriculo(db, &input))
                {
                case 0:
                    strcpy(titulo,"Articulos"); strcpy(mensaje,"Articulo Ingrsado");
                    input = (struct Articulo){0, "", "", 0.0f, ""};                     //Re-Incialzar el struct Articulo "input"
                    break;
                case 1:
                    strcpy(titulo,"Error"); strcpy(mensaje,"Datos Invalidos");
                    break;
                case 2:
                    strcpy(titulo,"Error"); strcpy(mensaje,"Error en la base de datos");
                    break;
                }
                //actualizar lista
                free(Lista);
                LargoLista=CantidadRows(db);
                Lista=malloc(LargoLista * sizeof(struct Articulo));
                CargarLista(db, Lista);
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
            GuiTextBox((Rectangle){90, 130, 200, 40}, consulta.Nombre, 50, EditConsultaNombre);
            //precio
            GuiLabel((Rectangle){25, 180, 200, 40}, "Precio");
            GuiValueBoxFloat((Rectangle){90, 180, 200, 40}, "", consulta.PrecioStr, &consulta.Precio, EditConsultaPrecio);
            //Consultar / Confirmar
            if (!editMode)
            {
                if (GuiButton((Rectangle){90, 230, 200, 40}, "Consultar") && !MensajeActivo) 
                {
                
                    switch (ConsultarAriculo(db, &consulta))
                    {
                    case 0:
                        consulatdo = true;
                        break;
                    case 1:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Datos Invalidos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""}; 
                        break;
                    case 2:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Error en la base de datos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""}; 
                        break;
                    }
                    snprintf(consulta.PrecioStr, sizeof(consulta.PrecioStr), "%.2f", consulta.Precio);
                }
            }
            else
            {
                if (GuiButton((Rectangle){90, 230, 200, 40}, "Confirmar") && !MensajeActivo) 
                {
                    switch (EditarArticulo(db, &consulta))
                    {
                    case 0:
                        MensajeActivo=true;
                        strcpy(titulo,"Editar"); strcpy(mensaje,"Articulo Editado");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""};
                        editMode = false;
                        //actualizar lista
                        free(Lista);
                        LargoLista=CantidadRows(db);
                        Lista=malloc(LargoLista * sizeof(struct Articulo));
                        CargarLista(db, Lista);
                        break;
                    case 1:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Datos Invalidos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""}; 
                        editMode = false;
                        break;
                    case 2:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Error en la base de datos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""};
                        editMode = false;
                        break;
                    }
                    consulatdo = false;
                }
            }
            
            //Editar
            GuiGroupBox((Rectangle){310, 60, 300, 140}, "Editar/borrar");
            if (GuiButton((Rectangle){390, 80, 200, 40}, "Editar") && !MensajeActivo) 
            {
                if (consulatdo)
                {
                    editMode = true; 
                }
                else
                {
                    MensajeActivo=true;
                    strcpy(titulo,"Error"); strcpy(mensaje,"Consulte primero");
                }
                
                
            }
            if (GuiButton((Rectangle){390, 130, 200, 40}, "Borrar") && !MensajeActivo) 
            {
                if (consulatdo)
                {
                    switch (BorrarArticulo(db, &consulta))
                    {
                    case 0:
                        MensajeActivo=true;
                        strcpy(titulo,"Borrar"); strcpy(mensaje,"Articulo Borrado");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""};
                        //actualizar lista
                        free(Lista);
                        LargoLista=CantidadRows(db);
                        Lista=malloc(LargoLista * sizeof(struct Articulo));
                        CargarLista(db, Lista);
                        break;
                    case 1:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Datos Invalidos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""}; 
                        break;
                    case 2:
                        MensajeActivo=true;
                        strcpy(titulo,"Error"); strcpy(mensaje,"Error en la base de datos");
                        consulta =(struct Articulo){0, "", "", 0.0f, ""};
                        break;
                    }
                    consulatdo = false;
                }
                else
                {
                    MensajeActivo=true;
                    strcpy(titulo,"Error"); strcpy(mensaje,"Consulte primero");
                }
            }

        }
        else if (currentTab == 2) 
        {
            // Lógica de desplazamiento
            if (IsKeyDown(KEY_UP)) scroll.y += 2; // Desplazar hacia abajo
            if (IsKeyDown(KEY_DOWN)) scroll.y -= 2;   // Desplazar hacia arriba
            
       
            // Borde la tabla
            GuiGroupBox((Rectangle){10, 60, 605, 330}, "Listado");
            // Encabezados de la tabla
            DrawRectangle(10, 65, 605, 25, GRAY);
            DrawText("ID", 20, 70, 20, BLACK);
            DrawText("Nombre", 100, 70, 20, BLACK);
            DrawText("Precio", 300, 70, 20, BLACK);
            BeginScissorMode(10,90, 605, 300); // Área visible de la tabla
            {
                // Filas de la tabla
                for (int i = 0; i < LargoLista; i++)   
                {
                    float y = 40 + i * (alturaFila + espacioFilas) + scroll.y;
                    DrawText(Lista[i].IdStr, 20, y, 20, BLACK);
                    DrawText(Lista[i].Nombre, 100, y, 20, BLACK);
                    DrawText(Lista[i].PrecioStr, 300, y, 20, BLACK);
                }
            }
            EndScissorMode();
        }
        if (MensajeActivo)
        {
            int result = GuiMessageBox((Rectangle){ 187.5f, 150.0f, 250, 100 },
                titulo, mensaje, "OK");

            if (result >= 0)
            {
                MensajeActivo = false;
                strcpy(titulo,""); strcpy(mensaje,"");
            } 
        }

        EndDrawing();
    }
    free(Lista);
    sqlite3_close(db);
    CloseWindow();
    return 0;
}