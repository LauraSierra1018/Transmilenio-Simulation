#include "Grafo.h"
#include <iostream>
#include <string>
#include <locale>
#include <iomanip>

int main() {
    // Set locale to properly display accented characters
    std::setlocale(LC_ALL, "");
    
    Grafo transmi;
    
    // Cargar la red desde los archivos de nodos y pesos (ponderaciones)
    if (!transmi.cargarDesdeArchivos("nodos.txt", "ponderacion.txt")) {
        std::cerr << "Error al cargar";
        return 1;
    }
    
    // menú
    bool salir = false;
    while (!salir) {
        std::cout << "\nAnalisis TransMilenio\n";
        std::cout << "1. Buscar camino mas corto entre estaciones\n";
        std::cout << "2. Listar todas las estaciones\n";
        std::cout << "3. Ver vecinos de una estacion\n";
        std::cout << "4. Salir\n";
        std::cout << "Opcion: ";
        
        int op;
        std::cin >> op;
        std::cin.ignore(); 
        
        switch (op) {
            case 1: {
                std::string estOrigen, estDestino;
                std::cout << "Estacion de origen: ";
                std::getline(std::cin, estOrigen);
                std::cout << "Estacion de destino: ";
                std::getline(std::cin, estDestino);
                
                Vector<int> ruta = transmi.Dijkstra(estOrigen, estDestino);
                
                if (ruta.getSize() == 0) {
                    std::cout << "No se encontro camino entre " << estOrigen << " y " << estDestino << "\n";
                } else {
                    double dist = transmi.getDistanciaCamino(ruta);
                    std::cout << std::fixed << std::setprecision(1);
                    std::cout << "Camino mas corto (" << dist << " min):\n";
                    
                    for (int i = 0; i < ruta.getSize(); i++) {
                        std::cout << transmi.getNombreEstacion(ruta.get(i));
                        if (i < ruta.getSize() - 1) {
                            std::cout << " -> ";
                        }
                    }
                    std::cout << "\n";
                }
                break;
            }
            case 2: {
                std::cout << "\nLista de Estaciones\n";
                for (int i = 0; i < transmi.getNumEstaciones(); i++) {
                    std::cout << i+1 << ". " << transmi.getNombreEstacion(i) << "\n";
                }
                break;
            }
            case 3: {
                std::string est;
                std::cout << "Nombre de la estacion: ";
                std::getline(std::cin, est);
                
                Vector<std::string> vecinos = transmi.getNombresVecinos(est);
                
                if (vecinos.getSize() == 0) {
                    std::cout << "No se encontro la estacion o no tiene conexiones.\n";
                } else {
                    std::cout << "Estaciones conectadas directamente a " << est << ":\n";
                    for (int i = 0; i < vecinos.getSize(); i++) {
                        std::cout << "- " << vecinos.get(i) << "\n";
                    }
                }
                break;
            }
            case 4:
                salir = true;
                break;
            default:
                std::cout << "Opcion no valida.";
        }
    }
    
    std::cout << "Fin";
    return 0;
}
