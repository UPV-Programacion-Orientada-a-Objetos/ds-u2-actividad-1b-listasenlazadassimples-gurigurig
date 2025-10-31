#ifndef SENSORES_H
#define SENSORES_H

#include "SensorBase.h"
#include "ListaSensor.h"

/**
 * @brief Sensor que maneja lecturas de temperatura (tipo float)
 * Contiene una lista genérica ListaSensor<float>
 */
class SensorTemperatura : public SensorBase {
private:
    ListaSensor<float> historial;

public:
    SensorTemperatura(const char* nom);
    ~SensorTemperatura();
    
    void registrarLectura(float valor);
    int procesarLectura() override;
    void imprimirInfo() const override;
};

/**
 * @brief Sensor que maneja lecturas de presión (tipo int)
 * Contiene una lista genérica ListaSensor<int>
 */
class SensorPresion : public SensorBase {
private:
    ListaSensor<int> historial;

public:
    SensorPresion(const char* nom);
    ~SensorPresion();
    
    void registrarLectura(int valor);
    int procesarLectura() override;
    void imprimirInfo() const override;
};

#endif // SENSORES_H
