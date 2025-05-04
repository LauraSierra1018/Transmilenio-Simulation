#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#endif // FUNCIONES_H

#include <sstream>
#include <string>
#include <type_traits>

using namespace std;

#ifndef FUNCIONES_TO_STRING
#define FUNCIONES_TO_STRING

// Verificación si un tipo tiene un método to_string()
template <typename, typename = void>
struct has_to_string : std::false_type {};

template <typename T>
struct has_to_string<T, std::void_t<decltype(std::declval<T>().to_string())>> 
    : std::is_same<decltype(std::declval<T>().to_string()), std::string> {};

#endif // FUNCIONES_H


// Representa un nodo individual en la lista enlazada
template <typename T>
class Nodo {
    T dato;     // El dato almacenado en el nodo
    Nodo* next; // Puntero al siguiente nodo en la lista

    // --- Funciones auxiliares para convertir a string (privadas) ---
    // Plantilla general para tipos aritméticos
    template <typename U>
    typename std::enable_if<std::is_arithmetic<U>::value, std::string>::type
    convert_to_string(const U& value) const {
        return std::to_string(value);
    }

    // Especialización para std::string
    template <typename U>
    typename std::enable_if<std::is_convertible<U, std::string>::value, std::string>::type
    convert_to_string(const U& value) const {
        return value;
    }

    // Versión para objetos con método .to_string()
    template <typename U>
    typename std::enable_if<has_to_string<U>::value, std::string>::type
    convert_to_string(const U& value) const {
        return value.to_string();
    }

    // Versión por defecto (intenta usar operador<<)
    template <typename U>
    typename std::enable_if<!std::is_arithmetic<U>::value && !std::is_convertible<U, std::string>::value && !has_to_string<U>::value, std::string>::type
    convert_to_string(const U&) const {
        // Intenta usar el operador de inserción << si está disponible
         std::ostringstream oss_check;
         oss_check << dato; // Verifica si el miembro dato soporta <<
         if (!oss_check.fail()) {
             std::ostringstream oss;
             oss << dato;
             return oss.str();
         }
        return "[Nodo: Tipo no soportado para to_string]";
    }
    // --- Fin Funciones auxiliares ---
    
public:
    // Constructor: inicializa el nodo con un valor
    Nodo(T value) : dato(value), next(nullptr) {}

    // --- Regla de los Cinco (versiones por defecto para Nodo) ---
    // (Generalmente suficientes si T es copiable/movible y no hay recursos propios que gestionar)
    Nodo(const Nodo<T>& other) = default;          // Constructor de copia
    Nodo(Nodo<T>&& other) noexcept = default; // Constructor de movimiento
    Nodo<T>& operator=(const Nodo<T>& other) = default; // Asignación por copia
    Nodo<T>& operator=(Nodo<T>&& other) noexcept = default; // Asignación por movimiento
    // Destructor (por defecto, no hace nada especial para Nodo)
    ~Nodo() = default;
    // --- Fin Regla de los Cinco (Nodo) ---

    // Convierte el dato del nodo a string
    std::string to_string() const { 
        return convert_to_string(dato);
    }
    
    // Obtiene el dato almacenado (const)
    T getDato() const { 
        return dato;
    }
    
    // Establece el dato del nodo
    void setDato(T d){ 
        dato = d;
    }
    
    // Obtiene el puntero al siguiente nodo (const)
    Nodo* getNext() const { 
        return next;
    }

    // Obtiene el puntero al siguiente nodo (no const, permite modificar la lista)
    Nodo* getNext() {
        return next;
    }
    
    // Establece el puntero al siguiente nodo
    void setNext(Nodo* n){ 
        next = n;
    }
    
    // Operador de inserción para imprimir con std::cout
    friend std::ostream& operator<<(std::ostream& os, const Nodo<T>& obj) {
        return os << obj.to_string();
    }
    
}; // Fin clase Nodo


// Declaración adelantada para la función amiga operator<< en LinkedList
template <typename T> class LinkedList;
template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& obj);

// Implementación de una lista enlazada simple
template <typename T>
class LinkedList {
private:
    
    Nodo<T>* head; // Puntero al primer nodo de la lista
    int size;     // Número de nodos en la lista

    // Función auxiliar: Libera todos los nodos de la lista
    void clear() {
         while (head) {
            Nodo<T>* temp = head;
            head = head->getNext();
            delete temp; // Libera cada nodo
        }
        size = 0;
        head = nullptr; // Asegura que head sea null después de limpiar
    }

    // Función auxiliar: Copia profundamente desde otra lista
    void copyFrom(const LinkedList<T>& other) {
        if (!other.head) {
            head = nullptr;
            size = 0;
            return;
        }

        // Copia el primer nodo
        head = new Nodo<T>(other.head->getDato());
        Nodo<T>* current = head;
        Nodo<T>* otherCurrent = other.head->getNext();
        size = 1;

        // Copia los nodos restantes
        while(otherCurrent) {
            Nodo<T>* newNode = new Nodo<T>(otherCurrent->getDato());
            current->setNext(newNode);
            current = newNode;
            otherCurrent = otherCurrent->getNext();
            size++;
        }
    }

public:
    // Constructor por defecto
    LinkedList() : head(nullptr), size(0) {}

    // Destructor: Limpia la lista para evitar fugas de memoria
    ~LinkedList() {
        clear();
    }

    // --- Regla de los Cinco (LinkedList) ---

    // Constructor de Copia (Copia Profunda)
    LinkedList(const LinkedList<T>& other) : head(nullptr), size(0) {
        copyFrom(other);
    }

    // Operador de Asignación por Copia (Copia Profunda)
    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this != &other) { // Evita auto-asignación
            clear(); // Libera recursos actuales
            copyFrom(other); // Copia desde other
        }
        return *this;
    }

    // Constructor de Movimiento
    LinkedList(LinkedList<T>&& other) noexcept : head(other.head), size(other.size) {
        // Transfiere propiedad de los recursos
        other.head = nullptr;
        other.size = 0;
    }

    // Operador de Asignación por Movimiento
    LinkedList<T>& operator=(LinkedList<T>&& other) noexcept {
         if (this != &other) { // Evita auto-asignación
            clear(); // Libera recursos actuales
            
            // Transfiere propiedad de los recursos
            head = other.head;
            size = other.size;

            // Deja other en estado válido y vacío
            other.head = nullptr;
            other.size = 0;
         }
         return *this;
    }
    // --- Fin Regla de los Cinco (LinkedList) ---

    // Agrega un nuevo nodo al final de la lista
    void add(T value) { 
        Nodo<T>* newNode = new Nodo<T>(value);
        if (!head) { // Si la lista está vacía
            head = newNode;
        } else { // Si la lista no está vacía
            Nodo<T>* temp = head;
            while (temp->getNext()) { // Recorre hasta el último nodo
                temp = temp->getNext();
            }
            temp->setNext(newNode); // Enlaza el último con el nuevo
        }
        size++; // Incrementa tamaño
    }
    
    // Obtiene el número de nodos en la lista
    int getSize() const { 
        return size;
    }

    // Obtiene el dato en el índice especificado (const)
    T get(int index) const { 
        if (index < 0 || index >= size) throw std::out_of_range("Índice fuera de rango en LinkedList::get");
        Nodo<T>* temp = head;
        for (int i = 0; i < index; i++) { // Recorre hasta el nodo deseado
            temp = temp->getNext();
        }
        return temp->getDato();
    }
    
    // Obtiene el puntero al nodo en el índice especificado (const)
    const Nodo<T>* getNodo(int index) const { 
        if (index < 0 || index >= size) throw std::out_of_range("Índice fuera de rango en LinkedList::getNodo const");
        Nodo<T>* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->getNext();
        }
        return temp;
    }

    // Obtiene el puntero al nodo en el índice especificado (no const)
    Nodo<T>* getNodo(int index) { 
        if (index < 0 || index >= size) throw std::out_of_range("Índice fuera de rango en LinkedList::getNodo");
        Nodo<T>* temp = head;
        for (int i = 0; i < index; i++) {
            temp = temp->getNext();
        }
        return temp;
    }

    // Inserta un nodo con 'value' en la posición 'index'
    void insert(int index, T value) { 
        if (index < 0 || index > size) throw std::out_of_range("Índice fuera de rango en LinkedList::insert");

        Nodo<T>* newNode = new Nodo<T>(value);
        if (index == 0) { // Insertar al principio
            newNode->setNext(head);
            head = newNode;
        } else { // Insertar en medio o al final
            Nodo<T>* temp = head;
            // Recorre hasta el nodo *anterior* al punto de inserción
            for (int i = 0; i < index - 1; i++) {
                temp = temp->getNext();
            }
            newNode->setNext(temp->getNext()); // Nuevo apunta al que seguía a temp
            temp->setNext(newNode);      // Temp apunta al nuevo
        }
        size++; // Incrementa tamaño
    }

    // Elimina el nodo en la posición 'index'
    void remove(int index) { 
        if (index < 0 || index >= size) throw std::out_of_range("Índice fuera de rango en LinkedList::remove");

        Nodo<T>* temp = nullptr; // Nodo a eliminar
        if (index == 0) { // Eliminar el primer nodo
            temp = head;
            head = head->getNext(); // Head apunta al segundo
        } else { // Eliminar nodo intermedio o final
            Nodo<T>* prev = head;
            // Recorre hasta el nodo *anterior* al que se va a eliminar
            for (int i = 0; i < index - 1; i++) {
                prev = prev->getNext();
            }
            temp = prev->getNext(); // temp es el nodo a eliminar
            prev->setNext(temp->getNext()); // prev se salta a temp
        }
        delete temp; // Libera la memoria del nodo eliminado
        size--; // Decrementa tamaño
    }

    // Imprime la lista a la consola
    void print() const { 
        std::cout << to_string(); // Usa to_string para consistencia
    }
    
    // Operador[] para acceder al puntero del nodo (no const)
    // Permite modificar el nodo directamente, usar con cuidado.
    Nodo<T>* operator[](int index) { 
        return getNodo(index);
    }

    // Operador[] para acceder al puntero del nodo (const)
    const Nodo<T>* operator[](int index) const { 
        return getNodo(index);
    }
    
    // Convierte la lista completa a un string
    std::string to_string() const { 
        string s = "";
        Nodo<T>* temp = head;
        while (temp) {
            s = s + temp->to_string() + " -> "; // Agrega representación del nodo
            temp = temp->getNext();
        }
        s = s + "nullptr\n"; // Indica el final de la lista
        return s;
    }
    
    // Declaración de amistad para que operator<< pueda acceder a miembros privados (si fuera necesario)
    // En este caso, usa to_string() público, por lo que no es estrictamente necesario, pero es buena práctica.
    friend std::ostream& operator<< <>(std::ostream& os, const LinkedList<T>& obj);
    
    // Operador de asignación por copia definido arriba
    // Operador de asignación por movimiento definido arriba
    
    
}; // Fin clase LinkedList

// Definición de la función amiga operator<< para imprimir LinkedList
template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& obj) {
    return os << obj.to_string(); // Llama al método público to_string()
}
