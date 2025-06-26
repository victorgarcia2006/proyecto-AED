#include <iostream>
#include <string>
#include <fstream>
#include <cstring>

using namespace std;

struct Persona {
    string curp;
    string nombre;
    string direccion;
    string correo;
    string telefono;
    Persona *sig;
    Persona *ant;
};


struct DatosPersona{
    char curp[50];
    char nombre[100];
    char direccion[100];
    char correo[100];
    char telefono[20];
};
class ListaPersonas {
private:
    Persona *inicio;
    Persona *fin;

public:
    ListaPersonas() {
        inicio = fin = NULL;
    }

    void InsertarInicio(string curp, string nombre, string direccion, string correo, string telefono) {
        Persona *nuevo = new Persona;
        if (nuevo == NULL) {
            cout << "No se pudo asignar memoria\n";
            return;
        }

        nuevo->curp = curp;
        nuevo->nombre = nombre;
        nuevo->direccion = direccion;
        nuevo->correo = correo;
        nuevo->telefono = telefono;
        nuevo->sig = inicio;
        nuevo->ant = NULL;

        if (inicio != NULL) {
            inicio->ant = nuevo;
        } else {
            fin = nuevo;
        }
        inicio = nuevo;

        cout << "Persona insertada al inicio.\n";

         GuardarDatos(nuevo);
        cout << "Datos guardados en el archivo.\n";
    }

    void InsertarFinal(string curp, string nombre, string direccion, string correo, string telefono) {
        Persona *nuevo = new Persona;
        if (nuevo == NULL) {
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

        if (fin != NULL) {
            fin->sig = nuevo;
        } else {
            inicio = nuevo;
        }
        fin = nuevo;

        cout << "Persona insertada al final.\n";
        GuardarDatos(nuevo);
    }

    void Mostrar() {
        Persona *temp = inicio;
        if (inicio == NULL) {
            cout << "No hay personas registradas.\n";
            return;
        }

        cout << "\n--- Lista de Personas Registradas ---\n";
        while (temp != NULL) {
            cout << "CURP: " << temp->curp << endl;
            cout << "Nombre: " << temp->nombre << endl;
            cout << "Direccion: " << temp->direccion << endl;
            cout << "Correo: " << temp->correo << endl;
            cout << "Telefono: " << temp->telefono << endl;
            cout << "-----------------------------------\n";
            temp = temp->sig;
        }
    }

    void BuscarPorCURP(string curp) {
        Persona *temp = inicio;
        while (temp != NULL) {
            if (temp->curp == curp) {
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

    void EliminarPorCURP(string curp) {
        Persona *temp = inicio;
        while (temp != NULL && temp->curp != curp) {
            temp = temp->sig;
        }

        if (temp == NULL) {
            cout << "Persona no encontrada.\n";
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
    }

    //De aqui inicia el codigo de manejo con archivos
    void GuardarDatos(const Persona* persona){
    DatosPersona datos;

    strncpy (datos.curp, persona->curp.c_str(), sizeof(datos.curp));
    strncpy (datos.nombre, persona->nombre.c_str(), sizeof(datos.nombre));
    strncpy (datos.direccion, persona->direccion.c_str(), sizeof(datos.direccion));
    strncpy (datos.correo, persona->correo.c_str(), sizeof(datos.correo));  
    strncpy (datos.telefono, persona->telefono.c_str(), sizeof(datos.telefono));

    ofstream archivo("personas.dat", ios::binary | ios::app);
    if (!archivo) {
        cout << "Error al abrir el archivo para guardar los datos.\n";
        return;
    }

    archivo.write(reinterpret_cast<const char*>(&datos), sizeof(DatosPersona));
    archivo.close();
    }
    void Leer() {
        ifstream archivo("personas.dat", ios::binary);
        if (!archivo) {
            cout << "No se pudo abrir el archivo para leer los datos.\n";
            return;
        }

        DatosPersona datos;
        int contador = 0;
        
        // Limpiar la lista actual antes de cargar
        while (inicio != NULL) {
            Persona *temp = inicio;
            inicio = inicio->sig;
            delete temp;
        }
        fin = NULL;

        // Leer datos del archivo
        while (archivo.read(reinterpret_cast<char*>(&datos), sizeof(DatosPersona))) {
            // Crear nueva persona
            Persona *nuevo = new Persona;
            if (nuevo == NULL) {
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
            if (fin != NULL) {
                fin->sig = nuevo;
            } else {
                inicio = nuevo;
            }
            fin = nuevo;
            contador++;
        }

        archivo.close();
        cout << "Se cargaron " << contador << " registros desde el archivo.\n";
    }

    void Modificar(string curp){
        //abrir archivo en modo lectura/escritura binario
        fstream archivo("personas.dat", ios::in | ios::out | ios::binary); //in>leer, out>escribir, binary>indica que es un archivo binario y no de texto

        //validacion de apertura
        if(!archivo){
            cout <<"Error al abrir el archivo para modificar"<<endl;
            return;
        }
        
        //preparar variables donde se leeran registros y revisara si fue o no encontrado la curp
        DatosPersona datos;
        bool encontrado = false;

        
        while(!archivo.eof()){ //estar leyendo secuencialmente los registros antes de llegar a 
            streampos pos = archivo.tellg(); //guarda la posicion
            archivo.read(reinterpret_cast<char*>(&datos), sizeof(DatosPersona)); //leer un bloque del tamaño de struct

        if (archivo.eof()) break; //si se llega a eof salir (End Of File)

        if (string(datos.curp) == curp) { //revisar si ya se encontro la persona ingresada
            encontrado = true;
            cout << "\nRegistro encontrado:\n";
            //mostar los datos de la persona previo a la edicion
            cout << "Nombre: " << datos.nombre << endl;
            cout << "Direccion: " << datos.direccion << endl;
            cout << "Correo: " << datos.correo << endl;
            cout << "Telefono: " << datos.telefono << endl;

            char opcion;

            //permitirle al usuario un menu para elegir que quiere o no editar
            cout << "¿Deseas modificar el nombre? (s/n): ";
            cin >> opcion;
            cin.ignore();
            if (opcion == 's') {
                cout << "Nuevo nombre: ";
                cin.getline(datos.nombre, sizeof(datos.nombre));
            }

            cout << "¿Deseas modificar la direccion? (s/n): ";
            cin >> opcion;
            cin.ignore(); //limpiar buffer
            if (opcion == 's') {
                cout << "Nueva direccion: ";
                cin.getline(datos.direccion, sizeof(datos.direccion)); //guardar nueva informacion en la variable, re escribiendo nueva informacion
            }

            cout << "¿Deseas modificar el correo? (s/n): ";
            cin >> opcion;
            cin.ignore();
            if (opcion == 's') {
                cout << "Nuevo correo: ";
                cin.getline(datos.correo, sizeof(datos.correo));
            }

            cout << "¿Deseas modificar el telefono? (s/n): ";
            cin >> opcion;
            cin.ignore();
            if (opcion == 's') {
                cout << "Nuevo telefono: ";
                cin.getline(datos.telefono, sizeof(datos.telefono));
            }

            // Volver a la posición para sobrescribir
            archivo.seekp(pos);
            archivo.write(reinterpret_cast<const char*>(&datos), sizeof(DatosPersona)); //sobre escribir en la posicion de la persona que fue editada
            cout << "Registro actualizado exitosamente.\n";
            break;
        }
        }
    }

    ~ListaPersonas() {
        Persona *temp = inicio;
        while (temp != NULL) {
            Persona *sig = temp->sig;
            delete temp;
            temp = sig;
        }
    }
};



void Menu() {
    cout << "\n=========== MENU PERSONAS ===========\n";
    cout << "1. Insertar informacion de persona (Crear)\n";
    cout << "2. Mostrar datos en archivo (Leer)\n";
    cout << "3. Modificar datos de persona por CURP (Editar)\n";
    cout << "4. Eliminar persona por CURP (Eliminar)\n";
    cout << "5. Salir\n";
    cout << "=====================================\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    ListaPersonas lista;
    int opcion;
    string curp, nombre, direccion, correo, telefono;

    do {
        Menu();
        cin >> opcion;
        cin.ignore(); // limpiar buffer

        switch (opcion) {
        case 1:
            //aqui va el codigo de citlali para crear una persona en el documento

            break;
        case 2:
            //aqui va el codigo de luis fernando en leer
            lista.Leer();
            break;
        case 3:
            //aqui va el modificar informacion de Ruby
            string Curp;
            cout<< "Ingrese la curp que quiere editar:"<<endl;
            getline(cin, Curp); //leer una linea de string
            lista.Modificar(Curp);
            break;
        case 4:
            //Aqui va el codigo de Victor para eliminar
            break;        
        case 5:
            //salir del programa
            cout<<"Saliendo del programa :D";
            opcion = 5;
            break;
        default:
            cout << "Opcion invalida. Intente nuevamente.\n";
        }

    } while (opcion != 5);

    return 0;
}
