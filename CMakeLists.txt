cmake_minimum_required(VERSION 3.10)
project(SistemaIoTPolimorfico)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCES
    main.cpp
    Implementaciones.cpp
)

set(HEADERS
    SensorBase.h
    ListaSensor.h
    Sensores.h
)

add_executable(SensorSystem ${SOURCES} ${HEADERS})

target_include_directories(SensorSystem PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(SensorSystem PRIVATE /W4)
else()
    target_compile_options(SensorSystem PRIVATE -Wall -Wextra -pedantic)
endif()

message(STATUS "Sistema de Gestión Polimórfica de Sensores IoT")
message(STATUS "Compilando con C++${CMAKE_CXX_STANDARD}")
message(STATUS "Archivos fuente: ${SOURCES}")
message(STATUS "Archivos header: ${HEADERS}")
