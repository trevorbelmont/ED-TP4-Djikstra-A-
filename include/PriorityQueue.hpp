#ifndef HEAP_HPP
#define HEAP_HPP


#include <iostream>

using namespace std;

template<typename T>
class PriorityQueue {
private:
    T* heap;
    int capacity;
    int size;

    // Funções auxiliares de caminhamento na "árvore" do heap
    int parent(int i);
    int leftChild(int i);
    int rightChild(int i);

    // Re-Heapfica pra cima
    void heapifyUp(int index);

    // Re-Heapifica pra baixo
    void heapifyDown(int index);

    // Função de realocação de memória
    void resize();

public:
    // Construtor básico
    PriorityQueue(int initialCapacity = 10);

    // Destrutor
    ~PriorityQueue();

    // função de inserção na heap
    void push(T element);

    // Remove o elemento no topo da fila retornando-o
    T pop();

    // Retorna o elemento no topo da heap sem removê-lo
    T top();

    // Retorna o elemento na posição pos do vetor.
    // Iteração não ordenada no vetor "bruto"
    T get(int pos);

    // Checa se a heap tá vazia
    bool empty();

    // Retorna o tamanho atual da heap
    int getSize();
};


// Funções auxiliares de caminhamento na "árvore" do heap
template<typename T>
int PriorityQueue<T>::parent(int i) { return (i - 1) / 2; }

template<typename T>
int PriorityQueue<T>::leftChild(int i) { return 2 * i + 1; }

template<typename T>
int PriorityQueue<T>::rightChild(int i) { return 2 * i + 2; }

// Re-Heapfica pra cima
template<typename T>
void PriorityQueue<T>::heapifyUp(int index) {
    while (index != 0 && heap[parent(index)] < heap[index]) {
        std::swap(heap[parent(index)], heap[index]);
        index = parent(index);
    }
}

// Re-Heapifica pra baixo
template<typename T>
void PriorityQueue<T>::heapifyDown(int index) {
    int largest = index;
    int left = leftChild(index);
    int right = rightChild(index);

    if (left < size && heap[left] > heap[largest]) {
        largest = left;
    }

    if (right < size && heap[right] > heap[largest]) {
        largest = right;
    }

    if (largest != index) {
        std::swap(heap[index], heap[largest]);
        heapifyDown(largest);
    }
}

// Função de realocação de memória
template<typename T>
void PriorityQueue<T>::resize() {
    capacity *= 2; // fator de realocação (sempre realoca o dobro do espaço): geometricamente
    T* newHeap = new T[capacity]; // O novo container do heap

    // Faz back-up das antigas informações no novo container
    for (int i = 0; i < size; ++i) {
        newHeap[i] = heap[i];
    }

    // Libera memória do array anterior
    delete[] heap;

    // Seta o ponteiro padrão pra apontar pra memória recém alocada
    heap = newHeap;
}

// Construtor básico
template<typename T>
PriorityQueue<T>::PriorityQueue(int initialCapacity) : capacity(initialCapacity), size(0) {
    heap = new T[capacity];
}

// El Destructor
template<typename T>
PriorityQueue<T>::~PriorityQueue() {
    delete[] heap; // desaloca memória do container (array)
}

// função de inserção na heap
template<typename T>
void PriorityQueue<T>::push(T element) {
    //Se a heap chegou no limite máximo realoca (o dobro)
    if (size == capacity) {
        resize();
    }

    heap[size] = element; //Coloca no array
    heapifyUp(size); //Re-heapifica
    ++size;
}

// Remove o elemento no topo da fila retornando-o
template<typename T>
T PriorityQueue<T>::pop() {
    // Checa vacuidade
    if (size == 0) {
        throw std::out_of_range("Impossível retirar de uma heap vazia : função pop() ");
    }
    //Remove, re-heapifica e retorna o elemento mínimo
    T root = heap[0];
    heap[0] = heap[--size];
    heapifyDown(0);
    return root;
}

// Retorna o elemento no topo da heap sem removê-lo
template<typename T>
T PriorityQueue<T>::top() {
    if (size == 0) {
        throw std::out_of_range("A heap está vazia : função top() ");
    }
    return heap[0];
}

// Retorna o elemento na posição pos do vetor.
// Iteração não ordenada no vetor "bruto"
template<typename T>
T PriorityQueue<T>::get(int pos) {
    // Checa válidade do índice
    if (pos >= size || pos < 0) {
        throw std::out_of_range("Posição não alocada : função get() ");
    }
    return heap[pos];
}

// Checa se a heap tá vazia
template<typename T>
bool PriorityQueue<T>::empty() {
    return size == 0;
}

// Retorna o tamanho atual da heap
template<typename T>
int PriorityQueue<T>::getSize() {
    return size;
}

#endif