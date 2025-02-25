# ArticleDB

Este proyecto es una aplicación de escritorio que utiliza SQLite para la gestión de una base de datos de artículos y Raylib para la interfaz gráfica de usuario.

## Dependencias

- [Raylib](https://www.raylib.com/)
- [SQLite](https://www.sqlite.org/)

## Uso

1. Descarga la [Aplicación](https://github.com/HernandezDev/ArticleDB/releases/download/1.0/ArticleDB_1.0.zip).
2. Descomprime el archivo ZIP descargado.
3. Ejecuta `ArticleDB.exe` desde la carpeta descomprimida.

## Funcionalidades

- **Carga de artículos**: Permite ingresar nuevos artículos a la base de datos.
- **Consulta por Id**: Permite consultar artículos por su ID, también editarlos o borrarlos.
- **Listado Completo**: Muestra una lista completa de los artículos en la base de datos.
- **Edición de artículos**: Permite editar los detalles de un artículo existente.
- **Eliminación de artículos**: Permite eliminar un artículo de la base de datos.
- **Desplazamiento en la lista**: Permite desplazarse por la lista de artículos utilizando las teclas de flecha.

## Validaciones de Datos

- **Nombre no vacío**: No se permite insertar o actualizar un artículo con un nombre vacío.
- **Precio positivo**: No se permite insertar o actualizar un artículo con un precio de cero o negativo.
- **Nombre único**: No se permite insertar un artículo con un nombre que ya exista en la base de datos.
- **No repetición de IDs**: Los IDs de los artículos son únicos y no se reutilizan, incluso si un artículo es borrado.

## Código Principal

El archivo principal del proyecto es [main.c](main.c), que contiene la lógica de la aplicación, la gestión de la base de datos y la interfaz gráfica de usuario.






