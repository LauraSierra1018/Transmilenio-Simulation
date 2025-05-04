#include "Grafo.h"
#include <iostream>
#include <limits>
#include <set>
#include <iomanip>

Grafo::Grafo() {}

bool Grafo::cargarDesdeCSV(const std::string& archivoNombre) {
    std::ifstream archivo(archivoNombre);
    if (!archivo.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivoNombre << std::endl;
        return false;
    }

    std::string linea;
    Vector<std::string> nomCabecera;
    Vector<Vector<double>> matrizTemp;
    int numLinea = 0;
    int tamFilaEsperado = -1;

    if (std::getline(archivo, linea)) {
        numLinea++;
        std::stringstream ss(linea);
        std::string celda;
        bool primeraCelda = true;
        while (std::getline(ss, celda, ',')) {
            if (!primeraCelda) {
                if (celda.empty()) {
                    std::cerr << "Error en linea " << numLinea << ": Nombre de estación vacío en la cabecera." << std::endl;
                    return false;
                }
                nomCabecera.push_back(celda);
            } else {
                primeraCelda = false;
            }
        }
        tamFilaEsperado = nomCabecera.getSize() + 1;  // +1 para la columna de nombres de estaciones
    } else {
        std::cerr << "Error: Archivo CSV vacío o no se pudo leer la cabecera." << std::endl;
        return false;
    }

    Vector<std::string> nomFilas;
    while (std::getline(archivo, linea)) {
        numLinea++;
        std::stringstream ss(linea);
        std::string celda;
        std::string nomFila;
        Vector<double> filaActual;
        int numCelda = 0;
        bool primeraCelda = true;

        while (std::getline(ss, celda, ',')) {
            if (primeraCelda) {
                nomFila = celda;
                if (nomFila.empty()){
                    std::cerr << "Error en linea " << numLinea << ": Nombre de estación vacío al inicio de la fila." << std::endl;
                    return false;
                }
                nomFilas.push_back(nomFila);
                primeraCelda = false;
            } else {
                if (!esEntero(celda)) {
                    std::cerr << "Error en linea " << numLinea << ", columna " << numCelda + 1 << ": Valor no entero ('" << celda << "'). Se espera 0 o 1." << std::endl;
                    return false;
                }
                int valor = std::stoi(celda);
                if (valor != 0 && valor != 1) {
                    std::cerr << "Advertencia en linea " << numLinea << ", columna " << numCelda + 1 << ": Valor inesperado " << valor << ". Se tratará como " << (valor != 0 ? 1 : 0) << "." << std::endl;
                    valor = (valor != 0 ? 1 : 0);
                }
                filaActual.push_back(static_cast<double>(valor));
            }
            numCelda++;
        }

        if (filaActual.getSize() != tamFilaEsperado) {
            std::cerr << "Error en linea " << numLinea << ": Número incorrecto de valores. Esperado: " << tamFilaEsperado << ", Encontrado: " << filaActual.getSize() << std::endl;
            return false;
        }

        matrizTemp.push_back(filaActual);
    }

    if (nomFilas.getSize() != nomCabecera.getSize()) {
        std::cerr << "Error: Número de filas (" << nomFilas.getSize() << ") no coincide con número de estaciones en cabecera (" << nomCabecera.getSize() << ")" << std::endl;
        return false;
    }

    bool mismoOrden = true;
    for (int i = 0; i < nomCabecera.getSize(); i++) {
        if (nomCabecera.get(i) != nomFilas.get(i)) {
            mismoOrden = false;
            std::cerr << "Advertencia: El orden de estaciones no coincide entre cabecera y filas." << std::endl;
            break;
        }
    }

    if (matrizTemp.getSize() != tamFilaEsperado - 1) {
        std::cerr << "Error: La matriz no es cuadrada. Filas: " << matrizTemp.getSize() << ", Columnas: " << tamFilaEsperado - 1 << std::endl;
        return false;
    }

    bool esSimetrica = true;
    for (int i = 0; i < matrizTemp.getSize(); i++) {
        for (int j = 0; j < i; j++) {
            if (matrizTemp.get(i).get(j) != matrizTemp.get(j).get(i)) {
                esSimetrica = false;
                std::cerr << "Advertencia: La matriz no es simétrica en posiciones [" << i << "," << j << "] y [" << j << "," << i << "]." << std::endl;
                std::cerr << "           Esto significa que el grafo es dirigido o hay un error en los datos." << std::endl;
                break;
            }
        }
        if (!esSimetrica) break;
    }

    nombresEstaciones = mismoOrden ? nomCabecera : nomFilas;
    matrizAdyacencia = matrizTemp;

    std::cout << "Grafo cargado exitosamente. " << getNumEstaciones() << " estaciones encontradas." << std::endl;
    return true;
}

bool Grafo::cargarDesdeArchivos(const std::string& archivoNodos, const std::string& archivoPesos) {
    // Limpiar el grafo actual
    nombresEstaciones = Vector<std::string>();
    matrizAdyacencia = Vector<Vector<double>>();
    
    // Cargar nodos (estaciones)
    std::ifstream fsNodos(archivoNodos);
    if (!fsNodos.is_open()) {
        std::cerr << "Error al abrir archivo de nodos: " << archivoNodos << std::endl;
        return false;
    }
    
    std::string linea;
    while (std::getline(fsNodos, linea)) {
        if (!linea.empty()) {
            nombresEstaciones.push_back(linea);
        }
    }
    fsNodos.close();
    
    // Inicializar matriz de adyacencia vacía
    for (int i = 0; i < nombresEstaciones.getSize(); i++) {
        Vector<double> fila;
        for (int j = 0; j < nombresEstaciones.getSize(); j++) {
            fila.push_back(0.0);
        }
        matrizAdyacencia.push_back(fila);
    }
    
    // Cargar pesos
    std::ifstream fsPesos(archivoPesos);
    if (!fsPesos.is_open()) {
        std::cerr << "Error al abrir archivo de pesos: " << archivoPesos << std::endl;
        return false;
    }
    
    int nodo1, nodo2;
    double peso;
    while (fsPesos >> nodo1 >> nodo2 >> peso) {
        if (nodo1 >= 0 && nodo1 < nombresEstaciones.getSize() && nodo2 >= 0 && nodo2 < nombresEstaciones.getSize()) {
            matrizAdyacencia[nodo1][nodo2] = peso;
            matrizAdyacencia[nodo2][nodo1] = peso; // grafo no dirigido
        } else {
            std::cerr << "Error: Índice de nodo fuera de rango: " << nodo1 << " o " << nodo2 << std::endl;
            return false;
        }
    }
    
    fsPesos.close();
    return true;
}

int Grafo::getNumEstaciones() const {
    return nombresEstaciones.getSize();
}

int Grafo::getIndiceEstacion(const std::string& estacion) const {
    for (int i = 0; i < nombresEstaciones.getSize(); i++) {
        if (nombresEstaciones.get(i) == estacion) {
            return i;
        }
    }
    return -1;
}

std::string Grafo::getNombreEstacion(int idx) const {
    if (idx >= 0 && idx < nombresEstaciones.getSize()) {
        return nombresEstaciones.get(idx);
    }
    return "";
}

bool Grafo::tieneArista(int idOrigen, int idDestino) const {
    if (idOrigen >= 0 && idOrigen < matrizAdyacencia.getSize() && 
        idDestino >= 0 && idDestino < matrizAdyacencia.getSize()) {
        return matrizAdyacencia.get(idOrigen).get(idDestino) > 0.0;
    }
    return false;
}

bool Grafo::tieneArista(const std::string& est1, const std::string& est2) const {
    int id1 = getIndiceEstacion(est1);
    int id2 = getIndiceEstacion(est2);
    if (id1 == -1 || id2 == -1) {
        return false;
    }
    return tieneArista(id1, id2);
}

Vector<int> Grafo::getIndicesVecinos(int idx) const {
    Vector<int> vecinos;
    
    if (idx < 0 || idx >= matrizAdyacencia.getSize()) {
        return vecinos;
    }
    
    for (int i = 0; i < matrizAdyacencia.get(idx).getSize(); i++) {
        if (matrizAdyacencia.get(idx).get(i) > 0.0) {
            vecinos.push_back(i);
        }
    }
    
    return vecinos;
}

Vector<std::string> Grafo::getNombresVecinos(const std::string& estacion) const {
    Vector<std::string> vecinos;
    int idx = getIndiceEstacion(estacion);
    
    if (idx == -1) {
        return vecinos;
    }
    
    Vector<int> indicesVecinos = getIndicesVecinos(idx);
    for (int i = 0; i < indicesVecinos.getSize(); i++) {
        vecinos.push_back(getNombreEstacion(indicesVecinos.get(i)));
    }
    
    return vecinos;
}

void Grafo::imprimirGrafo() const {
    int numEstaciones = getNumEstaciones();
    if (numEstaciones == 0) {
        std::cout << "Grafo vacío." << std::endl;
        return;
    }

    std::cout << "Matriz de Adyacencia:" << std::endl;

    std::cout << "        ";
    for (int i = 0; i < numEstaciones; ++i) {
        std::cout << nombresEstaciones.get(i).substr(0, 5) << "\t";
    }
    std::cout << std::endl;

    for (int i = 0; i < numEstaciones; ++i) {
        std::cout << nombresEstaciones.get(i).substr(0, 5) << "\t|";
        if (i < matrizAdyacencia.getSize()) {
             const Vector<double>& fila = matrizAdyacencia.get(i);
             for (int j = 0; j < numEstaciones; ++j) {
                 if (j < fila.getSize()) {
                     std::cout << std::fixed << std::setprecision(1) << "  " << fila.get(j) << "  \t";
                 } else {
                     std::cout << "  ?  \t";
                 }
             }
        } else {
            std::cout << "[Fila faltante]";
        }
        std::cout << std::endl;
    }
}

Vector<int> Grafo::Dijkstra(int idOrigen, int idDestino) const {
    int n = getNumEstaciones();
    Vector<int> ruta;
    
    if (idOrigen < 0 || idOrigen >= n || idDestino < 0 || idDestino >= n) {
        return ruta;
    }
    
    if (idOrigen == idDestino) {
        ruta.push_back(idOrigen);
        return ruta;
    }
    
    Vector<double> dist;
    Vector<int> prev;
    for (int i = 0; i < n; i++) {
        dist.push_back(std::numeric_limits<double>::infinity());
        prev.push_back(-1);
    }
    
    dist[idOrigen] = 0;
    
    std::set<std::pair<double, int>> cola;
    cola.insert({0, idOrigen});
    
    while (!cola.empty()) {
        int u = cola.begin()->second;
        cola.erase(cola.begin());
        
        if (u == idDestino) 
            break;
        
        Vector<int> vecinos = getIndicesVecinos(u);
        
        for (int i = 0; i < vecinos.getSize(); i++) {
            int v = vecinos.get(i);
            double peso = matrizAdyacencia.get(u).get(v);
            if (peso == 0.0) continue; // sin arista
            
            if (dist.get(u) != std::numeric_limits<double>::infinity() && 
                dist.get(u) + peso < dist.get(v)) {
                
                if (dist.get(v) != std::numeric_limits<double>::infinity()) {
                    cola.erase({dist.get(v), v});
                }
                
                dist[v] = dist.get(u) + peso;
                prev[v] = u;
                
                cola.insert({dist.get(v), v});
            }
        }
    }
    
    if (prev.get(idDestino) != -1 || idOrigen == idDestino) {
        int actual = idDestino;
        while (actual != -1) {
            ruta.push_back(actual);
            actual = prev.get(actual);
        }
        
        Vector<int> rutaInvertida;
        for (int i = ruta.getSize() - 1; i >= 0; i--) {
            rutaInvertida.push_back(ruta.get(i));
        }
        
        return rutaInvertida;
    }
    
    return ruta;
}

Vector<int> Grafo::Dijkstra(const std::string& estOrigen, const std::string& estDestino) const {
    int idOrigen = getIndiceEstacion(estOrigen);
    int idDestino = getIndiceEstacion(estDestino);
    
    if (idOrigen == -1 || idDestino == -1) {
        Vector<int> rutaVacia;
        return rutaVacia;
    }
    
    return Dijkstra(idOrigen, idDestino);
}

double Grafo::getDistanciaCamino(const Vector<int>& camino) const {
    if (camino.getSize() <= 1) {
        return 0;
    }
    
    double total = 0;
    for (int i = 0; i < camino.getSize() - 1; i++) {
        total += matrizAdyacencia.get(camino.get(i)).get(camino.get(i + 1));
    }
    
    return total;
}
