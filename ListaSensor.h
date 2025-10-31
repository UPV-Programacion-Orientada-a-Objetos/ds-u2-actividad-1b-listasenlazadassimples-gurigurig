#ifndef LISTASENSOR_H
#define LISTASENSOR_H

/**
 * @brief Nodo genérico de la lista enlazada
 * @tparam T Tipo de dato almacenado (int, float, double, etc.)
 */
template <typename T>
struct Nodo {
    T dato;
    Nodo<T>* siguiente;
    
    Nodo(T d) : dato(d), siguiente(nullptr) {}
};

/**
 * @brief Lista enlazada simple genérica
 * Implementa funciones de inserción, búsqueda y liberación
 * @tparam T Tipo de dato almacenado
 */
template <typename T>
class ListaSensor {
private:
    Nodo<T>* cabeza;
    int cantidad;

public:
    ListaSensor() : cabeza(nullptr), cantidad(0) {}
    
    ~ListaSensor() {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            Nodo<T>* siguiente = actual->siguiente;
            delete actual;
            actual = siguiente;
        }
    }
    
    /**
     * @brief Inserta un valor al final de la lista
     */
    void insertar(T valor) {
        Nodo<T>* nuevo = new Nodo<T>(valor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            Nodo<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        cantidad++;
    }
    
    /**
     * @brief Busca un valor en la lista
     * @return true si el valor existe, false si no
     */
    bool buscar(T valor) {
        Nodo<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->dato == valor) {
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }
    
    /**
     * @brief Elimina un valor específico de la lista
     * @return true si se eliminó, false si no se encontró
     */
    bool eliminar(T valor) {
        if (cabeza == nullptr) return false;
        
        // Si el valor está en la cabeza
        if (cabeza->dato == valor) {
            Nodo<T>* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            cantidad--;
            return true;
        }
        
        // Buscar en el resto de la lista
        Nodo<T>* actual = cabeza;
        while (actual->siguiente != nullptr) {
            if (actual->siguiente->dato == valor) {
                Nodo<T>* temp = actual->siguiente;
                actual->siguiente = temp->siguiente;
                delete temp;
                cantidad--;
                return true;
            }
            actual = actual->siguiente;
        }
        return false;
    }
    
    Nodo<T>* obtenerCabeza() const { return cabeza; }
    int obtenerCantidad() const { return cantidad; }
};

#endif // LISTASENSOR_H
