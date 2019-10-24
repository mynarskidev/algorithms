#include <iostream>
#include <fstream>
#include <vector>
#define MAX 214748364

using namespace std;

struct Edge
{
  public:
    int u;
    int v;
    int weight;

    Edge(int u, int v, int weight)
    {
        this->u = u;
        this->v = v;
        this->weight = weight;
    }

    void PrintEdge()
    {
        cout << u << " " << v << " " << weight << endl;
    }
};

void PrintResults(int *distances, int *previous, int size){
    cout << "Odleglosci: " << endl;
    for (int i = 0; i < size; i++){
        cout << distances[i];
        if (i < size - 1)
            cout << " ";
    }
    cout << endl;

    cout << "Poprzedniki: " << endl;
    for (int i = 0; i < size; i++){
        cout << previous[i];

        if (i < size - 1)
            cout << " ";
    }
    cout << endl << endl;
};

void BellmanFordFromMatrix(const char *file){
    cout << "Algorytm Bellmana-Forda: zapis macierzowy" << endl;

    ifstream input;
    input.open(file);

    int nodes;

    input >> nodes;
    cout << nodes;

    cout << endl;

    int **edges = new int *[nodes];
    for (int i = 0; i < nodes; i++){
        edges[i] = new int[nodes];
    }

    while (!input.eof()){
        for (int i = 0; i < nodes; i++){
            for (int j = 0; j < nodes; j++){
                input >> edges[i][j];
            }
        }
    }

    for (int i = 0; i < nodes; i++){
        for (int j = 0; j < nodes; j++){
            cout << edges[i][j];
            if (j < nodes - 1)
                cout << " ";
        }
        cout << endl;
    }
    input.close();

    int *distances = new int[nodes];
    int *previous = new int[nodes];

    for (int i = 0; i < nodes; i++){
        if (i == 0)
            distances[0] = 0;
        else
            distances[i] = MAX;

        previous[i] = -1;
    }

    for (int n = 1; n < nodes; n++){
        for (int i = 0; i < nodes; i++){
            for (int j = 0; j < nodes; j++){
                if (edges[i][j] == 0 || i == j)
                    continue;

                if (distances[j] > distances[i] + edges[i][j]){
                    distances[j] = distances[i] + edges[i][j];
                    previous[j] = i;
                }
            }
        }
    }
    PrintResults(distances, previous, nodes);
}

void BellmanFordFromTable(const char *file){
    cout << "Algorytm Bellmana-Forda: zapis tabelaryczny" << endl;

    ifstream input;
    input.open(file);

    int nodes;
    input >> nodes;

    int u, v, w;
    vector<Edge *> edges;
    while (!input.eof()){
        input >> u >> v >> w;

        struct Edge *e = new Edge(u, v, w);
        edges.push_back(e);
    }
    input.close();

    cout << nodes;
    cout << endl;

    for (auto &e : edges){
        e->PrintEdge();
    }

    int *distances = new int[nodes];
    int *previous = new int[nodes];

    for (int i = 0; i < nodes; i++){
        if (i == 0)
            distances[0] = 0;

        else
            distances[i] = MAX;

        previous[i] = -1;
    }

    for (int i = 1; i < nodes; i++){
        for (auto &e : edges){
            if (distances[e->v] > distances[e->u] + e->weight){
                distances[e->v] = distances[e->u] + e->weight;
                previous[e->v] = e->u;
            }
        }
    }

    PrintResults(distances, previous, nodes);
}

int main(int argc, const char *argv[]){
    BellmanFordFromTable("table.txt");
    BellmanFordFromMatrix("matrix.txt");
}
