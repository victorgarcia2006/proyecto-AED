#include <iostream>
#include <string>

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
            cout << "Dirección: " << temp->direccion << endl;
            cout << "Correo: " << temp->correo << endl;
            cout << "Teléfono: " << temp->telefono << endl;
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
                cout << "Dirección: " << temp->direccion << endl;
                cout << "Correo: " << temp->correo << endl;
                cout << "Teléfono: " << temp->telefono << endl;
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
    cout << "\n=========== MENÚ PERSONAS ===========\n";
    cout << "1. Insertar persona al inicio\n";
    cout << "2. Insertar persona al final\n";
    cout << "3. Mostrar todas las personas\n";
    cout << "4. Buscar persona por CURP\n";
    cout << "5. Eliminar persona por CURP\n";
    cout << "6. Salir\n";
    cout << "=====================================\n";
    cout << "Seleccione una opción: ";
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
            cout << "CURP: "; getline(cin, curp);
            cout << "Nombre: "; getline(cin, nombre);
            cout << "Dirección: "; getline(cin, direccion);
            cout << "Correo: "; getline(cin, correo);
            cout << "Teléfono: "; getline(cin, telefono);
            lista.InsertarInicio(curp, nombre, direccion, correo, telefono);
            break;
        case 2:
            cout << "CURP: "; getline(cin, curp);
            cout << "Nombre: "; getline(cin, nombre);
            cout << "Dirección: "; getline(cin, direccion);
            cout << "Correo: "; getline(cin, correo);
            cout << "Teléfono: "; getline(cin, telefono);
            lista.InsertarFinal(curp, nombre, direccion, correo, telefono);
            break;
        case 3:
            lista.Mostrar();
            break;
        case 4:
            cout << "Ingrese CURP a buscar: "; getline(cin, curp);
            lista.BuscarPorCURP(curp);
            break;
        case 5:
            cout << "Ingrese CURP a eliminar: "; getline(cin, curp);
            lista.EliminarPorCURP(curp);
            break;
        case 6:
            cout << "Saliendo del sistema...\n";
            break;
        default:
            cout << "Opción inválida. Intente nuevamente.\n";
        }

    } while (opcion != 6);

    return 0;
}
