#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cmath>
#include <iostream>
#include "Casal.hpp"
#include "PriorityQueue.hpp"

using namespace std;

// O TAD do Grafo
struct Vertex {
  int id;
  float x, y;
  bool explored;
  float dist, heuristic;
  int portals;
  int parent;

  PriorityQueue <Par<float, int>> adj;  // lista de adjacências ponderada // Par<peso, vertex_conexo>
  PriorityQueue <Par<float, int>> heurAdj; // Lista de adjacências com heurística pré-calculada
};

// Função para calcular a distância euclidiana entre dois pontos (x1, y1) e (x2, y2)
float euclidean(double x1, double y1, double x2, double y2);

class Graph {
 public:
  int size;   // número de vértices 
  Vertex* v;  // ponteiro para o array de vértices dinamicamente


  // Construtor do grafo de tamanho tam : variáveis são incializadas
  Graph(int tam);

  // Declara grafo de tamanho indefinido : variáveis NÃO são inicializadas
  Graph();
 
  // Reseta um grafo para tamanho específico : instancia os vértices e inicializa variáveis
  void reset(int tam);

  // Retorna variáveis do grafo pra um estadfo "virgem" de caminhamentos ou buscas em largura
  // Essa função é usada pra resetar o grafo após a primeira busca em largura (Djikstra) e prepará-lo para o A*
  void revirginate();

  // Cria arestas direcionadas na lista de adjacência do vértice com peso c
  bool makeDirectedWeightedEdge(int a, int b, float c);
  
  // Cria arestas direcionadas ponderando elas com o valor da heurística usada no A*
  bool makeHeuristicEdge(int a, int b, float c);
  
  // Imprime a lista de adjacências com letras ou números
  void print(bool alfabetic = false);

  // El DESTRUCTOR ¬¬ 
  ~Graph();
};


#endif
