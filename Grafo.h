#ifndef GRAFO_H
#define GRAFO_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <limits>
#include "Vector.h"

class Grafo {
private:
    Vector<std::string> nombresEstaciones;
    Vector<Vector<double>> matrizAdyacencia;
    
    bool esEntero(const std::string& s) const {
        if(s.empty()) return false;
        char* end = nullptr;
        long val = std::strtol(s.c_str(), &end, 10);
        return !*end && end != s.c_str() && val >= std::numeric_limits<int>::min() && val <= std::numeric_limits<int>::max();
    }

public:
    Grafo(); 
    ~Grafo() = default; 

    Grafo(const Grafo& other) = default;            
    Grafo(Grafo&& other) noexcept = default;        
    Grafo& operator=(const Grafo& other) = default; 
    Grafo& operator=(Grafo&& other) noexcept = default; 

    // Métodos principales
    bool cargarDesdeCSV(const std::string& archivoNombre);
    bool cargarDesdeArchivos(const std::string& archivoNodos, const std::string& archivoPesos);
    int getNumEstaciones() const;
    int getIndiceEstacion(const std::string& nombreEstacion) const;
    std::string getNombreEstacion(int indice) const;
    bool tieneArista(const std::string& estacion1, const std::string& estacion2) const;
    bool tieneArista(int indiceOrigen, int indiceDestino) const;
    Vector<int> getIndicesVecinos(int indice) const;
    Vector<std::string> getNombresVecinos(const std::string& nombreEstacion) const;
    void imprimirGrafo() const;

    // Algoritmo de Dijkstra
    Vector<int> Dijkstra(int indiceInicio, int indiceFin) const;
    Vector<int> Dijkstra(const std::string& estacionInicio, const std::string& estacionFin) const;
    double getDistanciaCamino(const Vector<int>& camino) const;
};

#endif // GRAFO_H
