#ifndef CASAL_HPP
#define CASAL_HPP

#include <iostream>

using namespace std;

//Declara e define a classe Par que é basicamente muito similar ao objeto pair da STL (Standard Libraries)


// Declaração da classe template Par
template<typename Ta, typename Tb>
class Par {
public:
    // Atributos públicos para armazenar os valores do par
    Ta first;
    Tb second;

    // Construtor padrão
    Par() : first(Ta()), second(Tb()) {}

    // Construtor que inicializa os valores do par
    Par(const Ta& a, const Tb& b) : first(a), second(b) {}

    // Sobrecarga do operador de igualdade
    bool operator==(const Par<Ta, Tb>& other) const {
        return (first == other.first) && (second == other.second);
    }

    // Sobrecarga do operador de desigualdade
    bool operator!=(const Par<Ta, Tb>& other) const {
        return !(*this == other);
    }

    // Sobrecarga do operador menor que
    bool operator<(const Par<Ta, Tb>& other) const {
        return (first < other.first) || ((other.first == first) && (second < other.second));
    }

    // Sobrecarga do operador menor ou igual
    bool operator<=(const Par<Ta, Tb>& other) const {
        return !(other < *this);
    }

    // Sobrecarga do operador maior que
    bool operator>(const Par<Ta, Tb>& other) const {
        return other < *this;
    }

    // Sobrecarga do operador maior ou igual
    bool operator>=(const Par<Ta, Tb>& other) const {
        return !(*this < other);
    }
};

// Sobrecarga do operador de saída para imprimir um par
template<typename Ta, typename Tb>
ostream& operator<<(ostream& os, const Par<Ta, Tb>& par) {
    os << "(" << par.first << ", " << par.second << ")";
    return os;
}

#endif // CASAL_HPP
