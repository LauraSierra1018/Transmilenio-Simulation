// Online C++ Compiler - Build, Compile and Run your C++ programs online in your favorite browser

#ifndef IOSTREAM
#define IOSTREAM
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>
#endif // FUNCIONES_H

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

template <typename T>
class Vector{
    T* v;         // Puntero al arreglo dinámico
    int capacity; // Capacidad actual del arreglo
    int size;     // Número actual de elementos
    
    
public:
    // Constructor
    Vector() : v(nullptr), capacity(10), size(0) {
        v = new T[capacity];
    }
    
    // Constructor con capacidad inicial
    Vector(int c) : v(nullptr), capacity(c > 0 ? c : 10), size(0) {
        if (capacity <= 0) capacity = 10; // Asegura capacidad positiva
        v = new T[capacity];
    }
    
    // Destructor: Libera la memoria del arreglo
    ~Vector(){
        delete[] v;
    }

    // --- Regla de los Cinco --- 

    // Constructor de Copia (Copia Profunda)
    Vector(const Vector<T>& other) : v(nullptr), capacity(other.capacity), size(other.size) {
        v = new T[capacity];
        for(int i = 0; i < size; ++i) {
            v[i] = other.v[i]; // Copia cada elemento
        }
    }

    // Operador de Asignación por Copia (Copia Profunda)
    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) { // Evita auto-asignación
            delete[] v; // Libera memoria actual
            
            capacity = other.capacity;
            size = other.size;
            v = new T[capacity]; // Reserva nueva memoria
            for(int i = 0; i < size; ++i) {
                v[i] = other.v[i]; // Copia elementos
            }
        }
        return *this;
    }

    // Constructor de Movimiento
    Vector(Vector<T>&& other) noexcept : v(other.v), capacity(other.capacity), size(other.size) {
        // Transfiere la propiedad de los recursos
        other.v = nullptr;
        other.size = 0;
        other.capacity = 0;
    }

    // Operador de Asignación por Movimiento
    Vector<T>& operator=(Vector<T>&& other) noexcept {
        if (this != &other) { // Evita auto-asignación
            delete[] v; // Libera memoria actual
            
            // Transfiere la propiedad de los recursos
            v = other.v;
            capacity = other.capacity;
            size = other.size;

            // Deja al objeto 'other' en un estado válido y vacío
            other.v = nullptr;
            other.size = 0;
            other.capacity = 0;
        }
        return *this;
    }
    // --- Fin Regla de los Cinco ---
    
    // Obtiene el número de elementos
    int getSize() const { 
        return size;
    }
    
    // Obtiene la capacidad actual
    int getCapacity() const { 
        return capacity;
    }
    
    // Obtiene el elemento en el índice i (versión const)
    T get(int i) const { 
        if(i>=0 && i<size){
            return v[i];
        } else {
            throw std::out_of_range("Índice fuera de rango en Vector::get");
        }
    }
    
    // Función auxiliar privada: Mueve elementos a la derecha para insertar
    void corrimientoDer(int i){ 
        //Corre todos los elementos desde la posición insert
        //una posición a la derecha
        if(size == capacity){
            resize();
        }
        for(int j=size; j>i; j--){
            v[j] = v[j-1];
        }
        // size se incrementa después en insert()
    }
    
    // Inserta el elemento 'd' en el índice 'i'
    void insert(T d, int i){ 
        if (i < 0 || i > size) { // Permite inserción al final (i == size)
             throw std::out_of_range("Índice fuera de rango en Vector::insert");
        }
        
        if(size == capacity){
            resize(); // Duplica capacidad si está lleno
        }
        
        // Desplaza elementos a la derecha
        for(int j = size; j > i; --j) {
            v[j] = v[j-1];
        }
        
        v[i] = d; // Inserta el elemento
        size++;   // Incrementa el tamaño
    }
    
    // Función auxiliar privada: Mueve elementos a la izquierda para eliminar
    void corrimientoIzq(int i){ 
        //Corre todos los elementos desde la izquierda hacia la posición i
        for(int j=i; j<size-1; j++){
            v[j] = v[j+1];
        }
        // size se decrementa después en removeI()
    }
    
    // Elimina el elemento en el índice 'i'
    void removeI(int i){ 
         if (i < 0 || i >= size) {
             throw std::out_of_range("Índice fuera de rango en Vector::removeI");
        }
        corrimientoIzq(i); // Desplaza elementos a la izquierda
        size--; // Decrementa el tamaño
    }
    
    // Busca el elemento 'd' y devuelve su índice (-1 si no se encuentra)
    int find(T d) const { 
        int i=0;
        while(i<size && v[i]!=d){
            i++;
        }
        if(i<size){
            return i;
        }
        return -1; // No encontrado
    }
    
    // Duplica la capacidad del vector
    void resize(){ 
        capacity = (capacity == 0) ? 10 : capacity * 2; // Maneja capacidad inicial 0
        T* nv = new T[capacity]; // Crea nuevo arreglo más grande
        for(int i=0; i<size; i++){
            nv[i] = std::move(v[i]); // Mueve elementos (más eficiente si T lo soporta)
        }
        delete[] v; // Libera memoria antigua
        v = nv;     // Apunta al nuevo arreglo
    }
    
    // Agrega un elemento al final del vector
    void add(T d){ 
        if(size == capacity){
            resize(); // Duplica capacidad si es necesario
        }
        v[size++] = d; // Agrega y luego incrementa size
    }

    // Alias para add (más común en std::vector)
    void push_back(T d) { 
        add(d);
    }
    
    // Imprime el vector a la consola
    void print() const { 
        std::cout << to_string() << std::endl;
    }
    
    // Acceso a elementos por índice (no const, permite modificar)
    T& operator[](size_t index) { 
        if (index >= size) {
            throw std::out_of_range("Índice fuera de rango en Vector::operator[]");
        }
        return v[index];
    }

    // Acceso a elementos por índice (const, solo lectura)
    const T& operator[](size_t index) const { 
        if (index >= size) {
            throw std::out_of_range("Índice fuera de rango en Vector::operator[] const");
        }
        return v[index];
    }

    
    // --- Métodos to_string (plantillas para diferentes tipos) ---
    // Versión para objetos que tienen su propio método .to_string()
    template <typename U = T>
    typename std::enable_if<has_to_string<U>::value, string>::type
    to_string() const { 
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            oss << v[i].to_string();
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Versión para punteros a objetos que tienen .to_string()
    template <typename U = T>
    typename std::enable_if<std::is_pointer<U>::value && has_to_string<typename std::remove_pointer<U>::type>::value, string>::type
    to_string() const { 
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            if (v[i]) {
                oss << v[i]->to_string();
            } else {
                oss << "null";
            }
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Versión para tipos numéricos (int, float, double, etc.)
    template <typename U = T>
    typename std::enable_if<std::is_arithmetic<U>::value, string>::type
    to_string() const { 
        ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < size; i++) {
            // Use std::to_string only if it's arithmetic, otherwise rely on stream operator
             if constexpr (std::is_arithmetic_v<U>) {
                 oss << std::to_string(v[i]);
             } else {
                 oss << v[i]; // Rely on potential operator<< overload for non-arithmetic U
             }
            if (i < size - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }

    // Versión por defecto (intenta usar operador<< si existe, sino mensaje de error)
    template <typename U = T>
    typename std::enable_if<!has_to_string<U>::value && !std::is_arithmetic<U>::value && !std::is_pointer<U>::value, string>::type
    to_string() const { 
        // Attempt to use stream insertion operator << if available
         std::ostringstream oss_check;
         oss_check << v[0]; // Check if the first element supports <<
         if (!oss_check.fail()) {
            ostringstream oss;
            oss << "[";
            for (size_t i = 0; i < size; ++i) {
                oss << v[i];
                 if (i < size - 1) oss << ", ";
            }
            oss << "]";
            return oss.str();
         }
        return "[Vector: No to_string() or operator<< available]";
    }
    // --- Fin Métodos to_string ---
    
    
    // Operador de inserción para imprimir con std::cout
    friend std::ostream& operator<<(std::ostream& os, const Vector<T>& obj) {
        // Usa la sobrecarga más apropiada de to_string()
        return os << obj.to_string();
    }
    
    // Operador de asignación por copia definido arriba

    // Operador de asignación por movimiento definido arriba
    
};