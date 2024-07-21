#include <cmath>
#include <iostream>
#include <limits>

#include "Casal.hpp"
#include "Graph.hpp"
#include "PriorityQueue.hpp"

using namespace std;

bool ALFABETICAL = false;
bool VERBOSE = false;
int ROOT1STDFS = 0;

void loadScene(Graph &G, float *sceneInfo) {
  int N, M, K, S, P; // respectivamente: vértices, arestas, portais, energia máxima, mana

  cin >> N >> M >> K;

  sceneInfo[0] = N;
  sceneInfo[1] = M;
  sceneInfo[2] = K;

  // re-instancia o grafo G com N vértices declarados;
  G.reset(N);

  // atribui as coordenadas de cada vértice
  float x, y;
  for (int i = 0; i < N; i++) {
    cin >> x >> y;
    G.v[i].x = x;
    G.v[i].y = y;
  }

// Calcula a distância herística de cada vértice pro sumidouro (terminal)
  for (int i = 0; i < N; i++) {
    G.v[i].heuristic =
        euclidean(G.v[i].x, G.v[i].y, G.v[N - 1].x, G.v[N - 1].y);
  }

  //Cria arestas ponderadas (simples e eursíticas)
  int a, b;
  float c;
  for (int i = 0; i < M; i++) {
    cin >> a >> b;

    // Calcula o peso normal da aresta (distância entre as clareiras)
    c = euclidean(G.v[a].x, G.v[a].y, G.v[b].x, G.v[b].y);

    // Cria aresta ponderada simples 
    G.makeDirectedWeightedEdge(a, b, c);

    // Cria aresta ponderada heurística (adicionando a distância pro sumidoro ao peso simples da aresta)
    G.makeHeuristicEdge(a, b, c + G.v[b].heuristic);
  }

// Cria portais (arestas com valor zero)
  for (int i = 0; i < K; i++) {
    cin >> a >> b;

    // Cria portal com peso zero (aresta simples de peso zero)
    G.makeDirectedWeightedEdge(a, b, 0);
    
    // Cria portal com peso heurístico
    G.makeHeuristicEdge(a, b, G.v[b].heuristic);
  }

  // Assinala energia e mana de Linque às variáveis de fora do escopo
  cin >> P >> S;
  sceneInfo[3] = P;
  sceneInfo[4] = S;
}

// Declaração do algoritmo de busca em largura de Djikistra
bool Djikstra(Graph &g, int s, int t);

// Declaração do algortimo de busca em largura por A*
bool AStar(Graph &g, int s, int t);

int main(int charc, char **charv) {
  if (charc > 1) {
    for (int i = 1; i < charc; i++) {
      if (charv[i][0] == '-') {
        switch (charv[i][1]) {
        case 'a':
          ALFABETICAL = true;
          break;
        case 'v':
          VERBOSE = true;
          break;
        case 'r':
          ROOT1STDFS = stoi(charv[i + 1]);
          if (VERBOSE) {
            cout << "ROOT of 1st DFS has been changed to vertex [";
            (ALFABETICAL) ? cout << char(65 + ROOT1STDFS)
                          : cout << ROOT1STDFS + 1;
            cout << "]." << endl;
          }

        default:
          break;
        }
      }
    }
  }

  // Carrega a cena
  Graph g;

  float scnInfo[5];
  loadScene(g, scnInfo);

  bool completeDjikstra = Djikstra(g, 0, g.size - 1);

  completeDjikstra = (g.v[g.size - 1].dist <= scnInfo[3] &&
                      g.v[g.size - 1].portals <= scnInfo[4]);

  if (VERBOSE) {
    cout << "G Djikstra: " << endl;
    g.print();
    cout << endl;
  }

  g.revirginate();

  bool completeAStar = AStar(g, 0, g.size - 1);
  completeAStar = (g.v[g.size - 1].dist <= scnInfo[3] &&
                   g.v[g.size - 1].portals <= scnInfo[4]);

  if (VERBOSE) {
    cout << "H AStar" << endl;
    g.print();
  }
  cout << completeDjikstra << ' ' << completeAStar << endl;
  
}

bool Djikstra(Graph &g, int s, int t) {
  g.v[s].dist = 0;
  g.v[s].explored = 1;
  bool reacheable = 0;

  PriorityQueue<Par<float, int>> fila;
  Par<float, int> source(0, s);
  fila.push(source);

  while (!fila.empty()) {
    Par<float, int> w = fila.top();
    fila.pop();

    // não testa se chegou por que queremos a menor distância

    for (int i = 0; i < g.v[w.second].adj.getSize(); i++) {
      Par pairU = g.v[w.second].adj.get(i);
      int u = pairU.second;

      if (g.v[u].dist > g.v[w.second].dist + pairU.first) {
        g.v[u].dist = g.v[w.second].dist + pairU.first;
        g.v[u].parent = w.second;
        g.v[u].explored = 1;

        if (pairU.first == 0) {
          g.v[pairU.second].portals = g.v[w.second].portals + 1;
        } else {
          g.v[pairU.second].portals = g.v[w.second].portals;
        }

        fila.push(pairU);
      }
    }
  }
  return g.v[t].explored;
}

bool AStar(Graph &g, int s, int t) {
  g.v[s].dist = 0;
  g.v[s].explored = 1;
  bool reacheable = 0;

  PriorityQueue<Par<float, int>> fila;
  Par<float, int> source(0, s);
  fila.push(source);

  while (!fila.empty()) {
    Par<float, int> w = fila.top();
    fila.pop();

    // não testa se chegou por que queremos a menor distância
    if (w.second == t) {
      return true;
    }

    for (int i = 0; i < g.v[w.second].heurAdj.getSize(); i++) {
      Par pairU = g.v[w.second].heurAdj.get(i);
      int u = pairU.second;

      // Só abre vértices não explorado (diferente do Djikstra) não atualizando
      // caminhos mais curtos
      if (g.v[u].dist > g.v[w.second].dist + pairU.first &&
          g.v[u].explored == 0) {
        g.v[u].dist = g.v[w.second].dist - g.v[u].heuristic + pairU.first;
        g.v[u].parent = w.second;
        g.v[u].explored = 1;

        // Se o peso da aresta for igual a heuristica a aresta é um portal
        if (pairU.first == g.v[pairU.second].heuristic) {
          g.v[pairU.second].portals = g.v[w.second].portals + 1;
        } else {
          g.v[pairU.second].portals = g.v[w.second].portals;
        }

        fila.push(pairU);
      }
    }
  }
  return g.v[t].explored;
}