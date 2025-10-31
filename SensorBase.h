#ifndef SENSORBASE_H
#define SENSORBASE_H

/**
 * @brief Clase base abstracta para todos los sensores
 * Define la interfaz común para el manejo polimórfico de sensores
 */
class SensorBase {
protected:
    char nombre[50]; // Identificador del sensor

public:
    /**
     * @brief Constructor
     * @param nom Nombre identificador del sensor
     */
    SensorBase(const char* nom);
    
    /**
     * @brief Destructor virtual para correcta liberación polimórfica
     */
    virtual ~SensorBase();
    
    /**
     * @brief Procesa la lectura del sensor (método virtual puro)
     * @return 0 si el procesamiento fue exitoso
     */
    virtual int procesarLectura() = 0;
    
    /**
     * @brief Imprime información del sensor (método virtual puro)
     */
    virtual void imprimirInfo() const = 0;
    
    /**
     * @brief Obtiene el nombre del sensor
     * @return Puntero al nombre del sensor
     */
    const char* obtenerNombre() const;
};

#endif // SENSORBASE_H
