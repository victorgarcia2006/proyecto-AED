/*
Alumnos:
- Víctor Alfredo García Calvillo
- Luis Fernando Escobedo Hernández
- Ruby Esmeralda López Correa
- Citlali García Espinoza
*/

#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

struct Persona
{
    string curp;
    string nombre;
    string direccion;
    string correo;
    string telefono;
    Persona *sig;
    Persona *ant;
};

struct DatosPersona
{ // guarda  todos los campos juntos como un bloque binario
    char curp[50];
    char nombre[100];
    char direccion[100];
    char correo[100];
    char telefono[20];
};
class ListaPersonas
{
private:
    Persona *inicio;
    Persona *fin;

public:
    ListaPersonas()
    {
        inicio = fin = NULL;
    }

    void InsertarInicio(string curp, string nombre, string direccion, string correo, string telefono)
    {
        Persona *nuevo = new Persona; // Crear un nuevo nodo de tipo Persona
                                      // Verificar si la memoria se asigno correctamente
        if (nuevo == NULL)
        {
            cout << "No se pudo asignar memoria\n";
            return;
        }

        // Asignar los valores a los campos del nuevo nodo
        nuevo->curp = curp;
        nuevo->nombre = nombre;
        nuevo->direccion = direccion;
        nuevo->correo = correo;
        nuevo->telefono = telefono;
        nuevo->sig = inicio;
        nuevo->ant = NULL;

        // Si la lista no está vacía, ajustar el puntero anterior del nodo inicial
        // para que apunte al nuevo nodo
        if (inicio != NULL)
        {
            inicio->ant = nuevo;
        }
        else
        {
            fin = nuevo;
        }
        inicio = nuevo;

        cout << "Persona insertada al inicio.\n";

        // Guardar los datos en el archivo
        // Llamar a la funcion GuardarDatos para guardar el nuevo nodo en el archivo
        GuardarDatos();
        system("pause");
    }

    void InsertarFinal(string curp, string nombre, string direccion, string correo, string telefono)
    {
        Persona *nuevo = new Persona;
        if (nuevo == NULL)
        {
            cout << "No se pudo asignar memoria\n";
            return;
        }

        nuevo->curp = curp;
        nuevo->nombre = nombre;
        nuevo->direccion = direccion;
        nuevo->correo = correo;
        nuevo->telefono = telefono;
        nuevo->sig = NULL;
        nuevo->ant = fin;

        if (fin != NULL)
        {
            fin->sig = nuevo;
        }
        else
        {
            inicio = nuevo;
        }
        fin = nuevo;

        cout << "Persona insertada al final.\n";
        GuardarDatos();
        system("pause");
    }

    void Mostrar()
    {
        Persona *temp = inicio;
        if (inicio == NULL)
        {
            cout << "No hay personas registradas.\n";
            return;
        }

        cout << "\n--- Lista de Personas Registradas ---\n";
        while (temp != NULL)
        {
            cout << "CURP: " << temp->curp << endl;
            cout << "Nombre: " << temp->nombre << endl;
            cout << "Direccion: " << temp->direccion << endl;
            cout << "Correo: " << temp->correo << endl;
            cout << "Telefono: " << temp->telefono << endl;
            cout << "-----------------------------------\n";
            temp = temp->sig;
        }
    }

    void BuscarPorCURP(string curp)
    {
        CargarDatos(); // Asegurarse de que los datos estén cargados antes de buscar
        Persona *temp = inicio;
        while (temp != NULL)
        {
            if (temp->curp == curp)
            {
                cout << "\nPersona encontrada:\n";
                cout << "CURP: " << temp->curp << endl;
                cout << "Nombre: " << temp->nombre << endl;
                cout << "Direccion: " << temp->direccion << endl;
                cout << "Correo: " << temp->correo << endl;
                cout << "Telefono: " << temp->telefono << endl;
                return;
            }
            temp = temp->sig;
        }
        cout << "Persona no encontrada.\n";
    }

    void EliminarPorCURP(string curp)
    {
        Persona *temp = inicio;
        while (temp != NULL && temp->curp != curp)
        {
            temp = temp->sig;
        }

        if (temp == NULL)
        {
            cout << "Persona no encontrada.\n";
            system("pause");
            return;
        }

        if (temp->ant != NULL)
            temp->ant->sig = temp->sig;
        else
            inicio = temp->sig;

        if (temp->sig != NULL)
            temp->sig->ant = temp->ant;
        else
            fin = temp->ant;

        delete temp;
        cout << "Persona eliminada por CURP.\n";

        // Llamar a la función que actualiza el archivo
        EliminarDato(curp);
    }

    // De aqui inicia el codigo de manejo con archivos
    //  Esta funcion guarda los datos de una persona en un archivo binario
    //  Recibe un puntero a una estructura Persona y guarda sus datos en un archivo llamado "personas.dat"
    //  Utiliza la estructura DatosPersona para almacenar los datos de forma binaria
    void GuardarDatos()
    {
        // Estructura auxiliar que usaremos para guardar los datos en el archivo
        DatosPersona datos;

        // Abrimos el archivo binario en modo escritura (ios::binary)
        // y en modo truncar (ios::trunc), para que se borre el contenido anterior
        ofstream archivo("personas.dat", ios::binary | ios::trunc);
        if (!archivo)
        {
            cout << "Error al abrir el archivo para guardar los datos.\n";
            return;
        }

        // Puntero temporal para recorrer la lista doblemente enlazada
        Persona *temp = inicio;

        // Recorremos toda la lista desde el inicio hasta el final
        while (temp != NULL)
        {
            // Limpiamos la estructura `datos` con ceros para evitar basura en los buffers
            memset(&datos, 0, sizeof(DatosPersona));

            // Copiamos los strings de cada campo al arreglo de caracteres de tamaño fijo
            // Usamos strncpy para no sobrepasar el límite de cada campo
            strncpy(datos.curp, temp->curp.c_str(), sizeof(datos.curp) - 1);
            strncpy(datos.nombre, temp->nombre.c_str(), sizeof(datos.nombre) - 1);
            strncpy(datos.direccion, temp->direccion.c_str(), sizeof(datos.direccion) - 1);
            strncpy(datos.correo, temp->correo.c_str(), sizeof(datos.correo) - 1);
            strncpy(datos.telefono, temp->telefono.c_str(), sizeof(datos.telefono) - 1);

            // Escribimos el contenido de la estructura `datos` en el archivo como bytes binarios
            archivo.write(reinterpret_cast<const char *>(&datos), sizeof(DatosPersona));

            // Avanzamos al siguiente nodo en la lista
            temp = temp->sig;
        }

        // Cerramos el archivo después de terminar el recorrido y escritura
        archivo.close();

        // Mensaje de confirmación
        cout << "Datos guardados correctamente en el archivo binario.\n";
    }

    // Funcion para cargar los datos desde el archivo binario sin mostrarlos, ya que se ejecutará al inicio del programa
    void CargarDatos()
    {
        ifstream archivo("personas.dat", ios::binary);
        if (!archivo)
        {
            cout << "No se pudo abrir el archivo para cargar datos.\n";
            return;
        }

        // 🧹 LIMPIAR la lista actual en memoria (RAM)
        Persona *actual = inicio;
        while (actual != NULL)
        {
            Persona *siguiente = actual->sig;
            delete actual;
            actual = siguiente;
        }
        inicio = fin = NULL;

        DatosPersona datos;
        while (archivo.read(reinterpret_cast<char *>(&datos), sizeof(DatosPersona)))
        {
            Persona *nuevo = new Persona;
            nuevo->curp = datos.curp;
            nuevo->nombre = datos.nombre;
            nuevo->direccion = datos.direccion;
            nuevo->correo = datos.correo;
            nuevo->telefono = datos.telefono;
            nuevo->sig = NULL;
            nuevo->ant = fin;

            if (fin != NULL)
            {
                fin->sig = nuevo;
            }
            else
            {
                inicio = nuevo;
            }
            fin = nuevo;
        }

        archivo.close();
    }

    void Leer()
    {
        ifstream archivo("personas.dat", ios::binary);
        if (!archivo)
        {
            cout << "No se pudo abrir el archivo para leer los datos.\n";
            return;
        }

        DatosPersona datos;
        int contador = 0;

        // Limpiar la lista actual antes de cargar
        while (inicio != NULL)
        {
            Persona *temp = inicio;
            inicio = inicio->sig;
            delete temp;
        }
        fin = NULL;

        // Leer datos del archivo
        while (archivo.read(reinterpret_cast<char *>(&datos), sizeof(DatosPersona)))
        {
            // Crear nueva persona
            Persona *nuevo = new Persona;
            if (nuevo == NULL)
            {
                cout << "No se pudo asignar memoria\n";
                continue;
            }

            // Convertir de char a string
            nuevo->curp = string(datos.curp);
            nuevo->nombre = string(datos.nombre);
            nuevo->direccion = string(datos.direccion);
            nuevo->correo = string(datos.correo);
            nuevo->telefono = string(datos.telefono);
            nuevo->sig = NULL;
            nuevo->ant = fin;

            // Insertar al final de la lista
            if (fin != NULL)
            {
                fin->sig = nuevo;
            }
            else
            {
                inicio = nuevo;
            }
            fin = nuevo;
            contador++;
        }
        // Mostrar los datos
        system("cls"); // Limpiar la consola para una mejor visualizacion
        cout << "\n--- Lista de Personas registradas ---\n";
        Persona *temp = inicio;
        while (temp != NULL)
        {
            cout << "CURP: " << temp->curp << endl;
            cout << "Nombre: " << temp->nombre << endl;
            cout << "Direccion: " << temp->direccion << endl;
            cout << "Correo: " << temp->correo << endl;
            cout << "Telefono: " << temp->telefono << endl;
            cout << "-------------------------\n";
            temp = temp->sig;
        }
        // Cerrar el archivo
        archivo.close();
        cout << "Se cargaron " << contador << " registros desde el archivo.\n";
        system("pause"); // Pausa para que el usuario vea los datos cargados
    }

    void Modificar(string curp)
    {
        // abrir archivo en modo lectura/escritura binario
        fstream archivo("personas.dat", ios::in | ios::out | ios::binary); // in>leer, out>escribir, binary>indica que es un archivo binario y no de texto

        // validacion de apertura
        if (!archivo)
        {
            cout << "Error al abrir el archivo para modificar" << endl;
            return;
        }

        // preparar variables donde se leeran registros y revisara si fue o no encontrado la curp
        DatosPersona datos;
        bool encontrado = false;

        while (!archivo.eof())
        {                                                                         // estar leyendo secuencialmente los registros antes de llegar a
            streampos pos = archivo.tellg();                                      // guarda la posicion
            archivo.read(reinterpret_cast<char *>(&datos), sizeof(DatosPersona)); // leer un bloque del tamaño de struct

            if (archivo.eof())
                break; // si se llega a eof salir (End Of File)

            if (string(datos.curp) == curp)
            { // revisar si ya se encontro la persona ingresada
                encontrado = true;
                cout << "\nRegistro encontrado:\n";
                // mostar los datos de la persona previo a la edicion
                cout << "Nombre: " << datos.nombre << endl;
                cout << "Direccion: " << datos.direccion << endl;
                cout << "Correo: " << datos.correo << endl;
                cout << "Telefono: " << datos.telefono << endl;

                char opcion;

                // permitirle al usuario un menu para elegir que quiere o no editar
                cout << "Deseas modificar el nombre? (s/n): ";
                cin >> opcion;
                cin.ignore();
                if (opcion == 's')
                {
                    cout << "Nuevo nombre: ";
                    cin.getline(datos.nombre, sizeof(datos.nombre));
                }

                cout << "Deseas modificar la direccion? (s/n): ";
                cin >> opcion;
                cin.ignore(); // limpiar buffer
                if (opcion == 's')
                {
                    cout << "Nueva direccion: ";
                    cin.getline(datos.direccion, sizeof(datos.direccion)); // guardar nueva informacion en la variable, re escribiendo nueva informacion
                }

                cout << "Deseas modificar el correo? (s/n): ";
                cin >> opcion;
                cin.ignore();
                if (opcion == 's')
                {
                    cout << "Nuevo correo: ";
                    cin.getline(datos.correo, sizeof(datos.correo));
                }

                cout << "Deseas modificar el telefono? (s/n): ";
                cin >> opcion;
                cin.ignore();
                if (opcion == 's')
                {
                    cout << "Nuevo telefono: ";
                    cin.getline(datos.telefono, sizeof(datos.telefono));
                }

                // Volver a la posición para sobrescribir
                archivo.seekp(pos);
                archivo.write(reinterpret_cast<const char *>(&datos), sizeof(DatosPersona)); // sobre escribir en la posicion de la persona que fue editada
                cout << "Registro actualizado exitosamente.\n";
                break;
            }
        }
    }

    void EliminarDato(string curp)
    {
        // Abrimos el archivo original de personas en modo binario de lectura
        ifstream archivoEntrada("personas.dat", ios::binary);
        if (!archivoEntrada)
        {
            cout << "No se pudo abrir el archivo para leer.\n";
            return;
        }

        // Creamos un archivo temporal donde guardaremos solo los registros válidos (los que no serán eliminados)
        ofstream archivoTemporal("temp.dat", ios::binary);
        if (!archivoTemporal)
        {
            cout << "No se pudo crear el archivo temporal.\n";
            archivoEntrada.close();
            return;
        }

        DatosPersona datos;      // Estructura auxiliar para leer los datos del archivo original
        bool encontrado = false; // Bandera para saber si se encontró el CURP que queremos eliminar

        // Leemos cada estructura del archivo original
        while (archivoEntrada.read(reinterpret_cast<char *>(&datos), sizeof(DatosPersona)))
        {
            // Si el CURP del registro actual coincide con el que queremos eliminar
            if (curp == string(datos.curp))
            {
                encontrado = true; // Marcamos que se encontró
                continue;          // Saltamos este registro (no lo copiamos al archivo temporal)
            }

            // Si no coincide, escribimos el registro en el archivo temporal
            archivoTemporal.write(reinterpret_cast<const char *>(&datos), sizeof(DatosPersona));
        }

        // Cerramos ambos archivos
        archivoEntrada.close();
        archivoTemporal.close();

        // Eliminamos el archivo original
        if (remove("personas.dat") != 0)
        {
            cout << "Error al eliminar el archivo original.\n";
            system("pause");
            return;
        }

        // Renombramos el archivo temporal como el nuevo archivo original
        if (rename("temp.dat", "personas.dat") != 0)
        {
            cout << "Error al renombrar el archivo temporal.\n";
            system("pause");
            return;
        }

        // Mensaje según el resultado
        if (encontrado)
        {
            cout << "Persona eliminada del archivo binario.\n";
        }
        else
        {
            cout << "CURP no encontrada en el archivo.\n";
        }
        system("pause"); // Pausa para que el usuario vea el resultado
    }

    ~ListaPersonas()
    {
        Persona *temp = inicio;
        while (temp != NULL)
        {
            Persona *sig = temp->sig;
            delete temp;
            temp = sig;
        }
    }
};

void Menu()
{
    system("cls"); // Limpiar la consola para una mejor visualizacion
    cout << "\n=========== MENU PERSONAS ===========\n";
    cout << "1. Insertar informacion de persona al inicio (Crear)\n";
    cout << "2. Insertar informacion de persona al final (Crear)\n";
    cout << "3. Mostrar datos en archivo (Leer)\n";
    cout << "4. Buscar persona por CURP (Leer)\n";
    cout << "5. Modificar datos de persona por CURP (Editar)\n";
    cout << "6. Eliminar persona por CURP (Eliminar)\n";
    cout << "7. Salir\n";
    cout << "=====================================\n";
    cout << "Seleccione una opcion: ";
}

int main()
{
    ListaPersonas lista;
    int opcion;
    string curp, nombre, direccion, correo, telefono;
    // Cargar datos desde el archivo al iniciar el programa
    lista.CargarDatos(); // Cargar datos desde el archivo al iniciar el programa
    do
    {
        Menu();
        cin >> opcion;
        cin.ignore();  // limpiar buffer
        system("cls"); // Limpiar la consola al inicio
        switch (opcion)
        {
        case 1:
            // aqui va el codigo de citlali para crear una persona en el documento
            cout << "=== Insertar Informacion de Persona al Inicio ===\n";
            cout << "Ingrese CURP: ";
            getline(cin, curp);
            cout << "Ingrese Nombre: ";
            getline(cin, nombre);
            cout << "Ingrese Direccion: ";
            getline(cin, direccion);
            cout << "Ingrese Correo: ";
            getline(cin, correo);
            cout << "Ingrese Telefono: ";
            getline(cin, telefono);
            lista.InsertarInicio(curp, nombre, direccion, correo, telefono);

            break;
        case 2:
            // aqui va el codigo de luis fernando para insertar al final
            cout << "=== Insertar Informacion de Persona al Final ===\n";
            cout << "Ingrese CURP: ";
            getline(cin, curp);
            cout << "Ingrese Nombre: ";
            getline(cin, nombre);
            cout << "Ingrese Direccion: ";
            getline(cin, direccion);
            cout << "Ingrese Correo: ";
            getline(cin, correo);
            cout << "Ingrese Telefono: ";
            getline(cin, telefono);
            lista.InsertarFinal(curp, nombre, direccion, correo, telefono);

            break;
        case 3:
            // aqui va el codigo de luis fernando en leer
            lista.Leer();
            break;
        case 4:
            cout << "Ingrese la CURP que desea buscar: ";
            getline(cin, curp);
            lista.BuscarPorCURP(curp);
            system("pause");
            break;
        case 5:
            // aqui va el modificar informacion de Ruby
            cout << "Ingrese la curp que quiere editar:" << endl;
            getline(cin, curp); // leer una linea de string
            lista.Modificar(curp);
            break;
        case 6:
            // Aqui va el codigo de Victor para eliminar
            cout << "Ingrese la CURP de la persona a eliminar: ";
            getline(cin, curp);
            lista.EliminarPorCURP(curp);
            break;
        case 7:
            // salir del programa
            cout << "Saliendo del programa :D";
            opcion = 7;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente.\n";
        }

    } while (opcion != 7);

    return 0;
}
