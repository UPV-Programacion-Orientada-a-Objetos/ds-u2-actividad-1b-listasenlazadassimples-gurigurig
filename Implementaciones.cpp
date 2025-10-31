#include "SensorBase.h"
#include "Sensores.h"
#include <cstring>
#include <iostream>

// ==================== SensorBase ====================
SensorBase::SensorBase(const char* nom) {
    strncpy(nombre, nom, 49);
    nombre[49] = '\0';
}

SensorBase::~SensorBase() {}

const char* SensorBase::obtenerNombre() const {
    return nombre;
}

// ==================== SensorTemperatura ====================
SensorTemperatura::SensorTemperatura(const char* nom) : SensorBase(nom) {}

SensorTemperatura::~SensorTemperatura() {
    std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
}

void SensorTemperatura::registrarLectura(float valor) {
    historial.insertar(valor);
    std::cout << "ID: " << nombre << ". Valor: " << valor << " (float)" << std::endl;
}

int SensorTemperatura::procesarLectura() {
    Nodo<float>* actual = historial.obtenerCabeza();
    if (actual == nullptr) {
        std::cout << "[" << nombre << "] (Temperatura): Sin lecturas para procesar." << std::endl;
        return 0;
    }
    
    // Encontrar el valor mínimo
    float min = actual->dato;
    float suma = 0.0f;
    int cuenta = 0;
    
    actual = historial.obtenerCabeza();
    while (actual != nullptr) {
        if (actual->dato < min) {
            min = actual->dato;
        }
        suma += actual->dato;
        cuenta++;
        actual = actual->siguiente;
    }
    
    // ELIMINAR el valor mínimo de la lista
    historial.eliminar(min);
    
    // Recalcular el promedio sin el valor mínimo
    float promedioSinMin = 0.0f;
    int cuentaSinMin = historial.obtenerCantidad();
    
    if (cuentaSinMin > 0) {
        actual = historial.obtenerCabeza();
        float sumaSinMin = 0.0f;
        while (actual != nullptr) {
            sumaSinMin += actual->dato;
            actual = actual->siguiente;
        }
        promedioSinMin = sumaSinMin / cuentaSinMin;
        
        std::cout << "[" << nombre << "] (Temperatura): Lectura más baja (" << min 
                  << ") eliminada. Promedio restante: " << promedioSinMin << "." << std::endl;
    } else {
        std::cout << "[" << nombre << "] (Temperatura): Todas las lecturas fueron eliminadas." << std::endl;
    }
    
    return 0;
}

void SensorTemperatura::imprimirInfo() const {
    std::cout << "[Sensor Temp] Promedio calculado sobre " 
              << historial.obtenerCantidad() << " lectura(s)." << std::endl;
}

// ==================== SensorPresion ====================
SensorPresion::SensorPresion(const char* nom) : SensorBase(nom) {}

SensorPresion::~SensorPresion() {
    std::cout << "  [Destructor Sensor " << nombre << "] Liberando Lista Interna..." << std::endl;
}

void SensorPresion::registrarLectura(int valor) {
    historial.insertar(valor);
    std::cout << "ID: " << nombre << ". Valor: " << valor << " (int)" << std::endl;
}

int SensorPresion::procesarLectura() {
    Nodo<int>* actual = historial.obtenerCabeza();
    if (actual == nullptr) {
        std::cout << "[" << nombre << "] (Presion): Sin lecturas para procesar." << std::endl;
        return 0;
    }
    
    int suma = 0;
    int cuenta = 0;
    
    while (actual != nullptr) {
        suma += actual->dato;
        cuenta++;
        actual = actual->siguiente;
    }
    
    float promedio = static_cast<float>(suma) / cuenta;
    std::cout << "[" << nombre << "] (Presion): Promedio de lecturas: " << promedio << "." << std::endl;
    
    return 0;
}

void SensorPresion::imprimirInfo() const {
    std::cout << "[Sensor Presion] Promedio calculado sobre " 
              << historial.obtenerCantidad() << " lectura(s)." << std::endl;
}
