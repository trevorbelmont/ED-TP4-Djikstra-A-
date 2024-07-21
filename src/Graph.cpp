
#include <cmath>
#include <iostream>

#include "Graph.hpp"
#include "Casal.hpp"
#include "PriorityQueue.hpp"

using namespace std;

// Função para calcular a distância euclidiana entre dois pontos (x1, y1) e (x2, y2)
float euclidean(double x1, double y1, double x2, double y2) {
  return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// Construtor do grafo de tamanho tam : variáveis são incializadas
Graph::Graph(int tam) {
  size = tam;
  v = new Vertex[size];
  for (int i = 0; i < size; i++) {
    v[i].id = i;
    v[i].dist = numeric_limits<float>::max();
    v[i].heuristic = numeric_limits<float>::max();
    v[i].portals = 0;
    v[i].explored = 0;
    v[i].parent = -1;
  }
}

// Declara grafo de tamanho indefinido : variáveis NÃO são inicializadas
Graph::Graph() {
  size = -1;
  v = nullptr;
}

// Reseta um grafo para tamanho específico : instancia os vértices e inicializa variáveis
void Graph::reset(int tam) {
 
  if (v != nullptr) {
     delete[] v;
  }
  size = tam;
  v = new Vertex[size];

  for (int i = 0; i < size; i++) {
    v[i].id = i;
    v[i].dist = numeric_limits<float>::max();
    v[i].heuristic = numeric_limits<float>::max();
    v[i].portals = 0;
    v[i].parent = -1;
    v[i].explored = 0;
  }
}

// Retorna variáveis do grafo pra um estadfo "virgem" de caminhamentos ou buscas em largura
// Essa função é usada pra resetar o grafo após a primeira busca em largura (Djikstra) e prepará-lo para o A*
void Graph::revirginate() {
  for (int i = 0; i < size; i++) {
    v[i].dist = numeric_limits<float>::max();
    v[i].portals = 0;
    v[i].explored = 0;
    v[i].parent = -1;
  }
}

// Cria arestas direcionadas na lista de adjacência do vértice com peso c
bool Graph::makeDirectedWeightedEdge(int a, int b, float c) {
  if ((a >= 0 && a < size) && (b >= 0 && b < size)) {  // o enunciado do TP não permite a = b
    Par<float, int> edge(c, b);                         // o peso vem primeiro para viabilizar auto-ordenação na ED set (que será usado no Djikstra)
    v[a].adj.push(edge);
    return true;
  } else {
    cout << "Invalid vertex in bool makeEdge(int a, int b, int c)!" << endl;
    cout << "directed edge(origin,destiny, cost):  (" << a << ',' << b << ',' << c << ')' << ".  Graph. size = " << size << endl;
    return false;
  }
}

// Cria arestas direcionadas ponderando elas com o valor da heurística usada no A*
bool Graph::makeHeuristicEdge(int a, int b, float c) {
  if ((a >= 0 && a < size) && (b >= 0 && b < size)) {  // o enunciado do TP não permite a = b
    Par<float, int> edge(c, b);                         // o peso vem primeiro para viabilizar auto-ordenação na ED set (que será usado no Djikstra)
    v[a].heurAdj.push(edge);
    return true;
  } else {
    cout << "Invalid vertex in bool makeEdge(int a, int b, int c)!" << endl;
    cout << "directed edge(origin,destiny, cost):  (" << a << ',' << b << ',' << c << ')' << ".  Graph. size = " << size << endl;
    return false;
  }
}

// Imprime a lista de adjacências com letras ou números
void Graph::print(bool alfabetic) {
  cout << "Listas de Adjacência:" << endl;

  if (alfabetic) {
    for (int i = 0; i < size; i++) {  // para cada vértice do grafo
      cout << '[' << char(65 + v[i].id) << " - " << v[i].dist << "]";
      for (int j = 0; j < v[i].adj.getSize(); j++) {  // imprime a lista de adjacência com letras
        Par x = v[i].adj.get(j);    
        cout << ' ' << char(65 + x.second) << '(' << x.first << ") ";
      }
      cout << "  ~cor: " << v[i].explored << "  par: " << char(v[i].parent) << endl;
    }
  } else if (alfabetic == false) {
    for (int i = 0; i < size; i++) {  // para cada vértice do grafo
      cout << '[' << v[i].id << "/" << v[i].heuristic << " - " << v[i].dist << "/" << v[i].portals << "]" << "\t";
      for (int j = 0; j < v[i].adj.getSize(); j++) {  // imprime a lista de adjacência com números
        Par x = v[i].adj.get(j);   
        cout << ' ' << x.second << '(' << x.first << ") ";
      }
      cout << "  ~cor: " << v[i].explored << "  par: " << v[i].parent << endl;
    }
  }
}

Graph::~Graph() {
   if (v != nullptr) {
  delete[] v;
   }
}
