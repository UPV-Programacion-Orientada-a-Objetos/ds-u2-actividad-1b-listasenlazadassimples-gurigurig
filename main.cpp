#include "SensorBase.h"
#include "Sensores.h"
#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#endif

/**
 * @brief Nodo para la lista de gestión polimórfica (almacena SensorBase*)
 */
struct NodoGeneral {
    SensorBase* sensor;
    NodoGeneral* siguiente;
    
    NodoGeneral(SensorBase* s) : sensor(s), siguiente(nullptr) {}
};

/**
 * @brief Lista de gestión polimórfica NO genérica
 * Almacena punteros a SensorBase* para manejo polimórfico
 */
class ListaGeneral {
private:
    NodoGeneral* cabeza;

public:
    ListaGeneral() : cabeza(nullptr) {}
    
    ~ListaGeneral() {
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            NodoGeneral* siguiente = actual->siguiente;
            std::cout << "[Destructor General] Liberando Nodo: " << actual->sensor->obtenerNombre() << std::endl;
            delete actual->sensor; // Libera el sensor (destructor virtual correcto)
            delete actual; // Libera el nodo
            actual = siguiente;
        }
        std::cout << "Sistema cerrado. Memoria limpia." << std::endl;
    }
    
    void insertar(SensorBase* sensor) {
        NodoGeneral* nuevo = new NodoGeneral(sensor);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoGeneral* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevo;
        }
        std::cout << "[Log] Insertando Nodo en " << sensor->obtenerNombre() << "." << std::endl;
    }
    
    SensorBase* buscar(const char* nombre) {
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            if (strcmp(actual->sensor->obtenerNombre(), nombre) == 0) {
                return actual->sensor;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }
    
    void procesarTodos() {
        std::cout << "\n--- Procesando Sensores ---" << std::endl;
        NodoGeneral* actual = cabeza;
        while (actual != nullptr) {
            std::cout << "-> Procesando Sensor " << actual->sensor->obtenerNombre() << "..." << std::endl;
            actual->sensor->procesarLectura();
            actual = actual->siguiente;
        }
    }
};

/**
 * @brief Configura y abre el puerto serial (multiplataforma)
 */
int abrirPuertoSerial(const char* puerto) {
#ifdef _WIN32
    HANDLE hSerial = CreateFileA(puerto, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error abriendo puerto serial " << puerto << std::endl;
        return -1;
    }
    
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return -1;
    }
    
    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        CloseHandle(hSerial);
        return -1;
    }
    
    return (int)(size_t)hSerial;
#else
    int fd = open(puerto, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        std::cerr << "Error abriendo puerto serial " << puerto << std::endl;
        return -1;
    }
    
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr(fd, &tty) != 0) {
        close(fd);
        return -1;
    }
    
    cfsetospeed(&tty, B9600);
    cfsetispeed(&tty, B9600);
    
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;
    tty.c_iflag &= ~IGNBRK;
    tty.c_lflag = 0;
    tty.c_oflag = 0;
    tty.c_cc[VMIN]  = 1;
    tty.c_cc[VTIME] = 5;
    
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~(PARENB | PARODD);
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;
    
    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        close(fd);
        return -1;
    }
    
    return fd;
#endif
}

/**
 * @brief Lee una línea del puerto serial
 */
bool leerLineaSerial(int fd, char* buffer, int maxLen) {
#ifdef _WIN32
    HANDLE hSerial = (HANDLE)(size_t)fd;
    DWORD bytesRead;
    char c;
    int i = 0;
    
    while (i < maxLen - 1) {
        if (ReadFile(hSerial, &c, 1, &bytesRead, NULL) && bytesRead > 0) {
            if (c == '\n') break;
            if (c != '\r') buffer[i++] = c;
        }
    }
    buffer[i] = '\0';
    return i > 0;
#else
    int i = 0;
    char c;
    while (i < maxLen - 1) {
        if (read(fd, &c, 1) > 0) {
            if (c == '\n') break;
            if (c != '\r') buffer[i++] = c;
        }
    }
    buffer[i] = '\0';
    return i > 0;
#endif
}

/**
 * @brief Cierra el puerto serial
 */
void cerrarPuertoSerial(int fd) {
#ifdef _WIN32
    CloseHandle((HANDLE)(size_t)fd);
#else
    close(fd);
#endif
}

/**
 * @brief Procesa datos recibidos del Arduino
 * Formato esperado: "TIPO:ID:VALOR"
 * Ejemplo: "T:T-001:25.3" o "P:P-105:1013"
 */
void procesarDatosArduino(const char* linea, ListaGeneral& lista) {
    char tipo[10], id[50], valorStr[50];
    
    if (sscanf(linea, "%[^:]:%[^:]:%s", tipo, id, valorStr) == 3) {
        SensorBase* sensor = lista.buscar(id);
        
        if (strcmp(tipo, "T") == 0) {
            float valor = atof(valorStr);
            if (sensor == nullptr) {
                sensor = new SensorTemperatura(id);
                lista.insertar(sensor);
                std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestión." << std::endl;
            }
            static_cast<SensorTemperatura*>(sensor)->registrarLectura(valor);
        }
        else if (strcmp(tipo, "P") == 0) {
            int valor = atoi(valorStr);
            if (sensor == nullptr) {
                sensor = new SensorPresion(id);
                lista.insertar(sensor);
                std::cout << "Sensor '" << id << "' creado e insertado en la lista de gestión." << std::endl;
            }
            static_cast<SensorPresion*>(sensor)->registrarLectura(valor);
        }
    }
}

int main() {
    std::cout << "--- Sistema IoT de Monitoreo Polimórfico ---\n" << std::endl;
    
    // IMPORTANTE: Cambia este puerto según tu sistema
    // Windows: "COM3", "COM4", etc.
    // Linux: "/dev/ttyUSB0", "/dev/ttyACM0", etc.
    const char* puerto = "/dev/ttyUSB0";
    
    std::cout << "Intentando conectar con Arduino en puerto: " << puerto << std::endl;
    int serialFd = abrirPuertoSerial(puerto);
    
    if (serialFd < 0) {
        std::cerr << "No se pudo abrir el puerto. Verifica la conexión del Arduino." << std::endl;
        std::cerr << "Para Windows usa 'COM3', para Linux '/dev/ttyUSB0' o '/dev/ttyACM0'" << std::endl;
        return 1;
    }
    
    std::cout << "Conexión establecida. Esperando datos del Arduino..." << std::endl;
    std::cout << "(Formato esperado: TIPO:ID:VALOR, ej: T:T-001:25.3)\n" << std::endl;
    
    ListaGeneral lista;
    char buffer[256];
    int contadorLecturas = 0;
    
    while (true) {
        if (leerLineaSerial(serialFd, buffer, sizeof(buffer))) {
            procesarDatosArduino(buffer, lista);
            contadorLecturas++;
            
            // Procesar después de cada 5 lecturas
            if (contadorLecturas >= 5) {
                lista.procesarTodos();
                contadorLecturas = 0;
                
                std::cout << "\n¿Continuar? (s/n): ";
                char opcion;
                std::cin >> opcion;
                if (opcion == 'n' || opcion == 'N') {
                    break;
                }
            }
        }
    }
    
    std::cout << "\n--- Liberación de Memoria en Cascada ---" << std::endl;
    cerrarPuertoSerial(serialFd);
    
    return 0;
}
