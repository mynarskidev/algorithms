#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include "limits.h"

using namespace std;

int max_flow = 0;

void show_matrix(int ** arr, int length){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length; j++){
            cout << arr[i][j];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool breadth_first(int ** rGraph, int s, int e, int * path, int N){
    bool visited[N];
    for(int i = 0; i < N; i++){
        visited[i] = false;
    }

    queue<int> q;
    q.push(s);
    visited[s] = true;
    path[s] = -1;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(int v = 0; v < N; v++){
            if(visited[v] == false && rGraph[u][v] > 0){
                q.push(v);
                path[v] = u;
                visited[v] = true;
            }
        }
    }

    return (visited[e] == true);
}

int min_flow_in_path(int * path, int s, int e, int ** rGraph) {
    int minF = INT_MAX;
    for(int v = e; v != s; v = path[v]){
        int u = path[v];
        minF = min(minF, rGraph[u][v]);
    }
    return minF;
}

void update_residual_edges(int * path, int **rGraph, int min_flow, int s, int e) {
    for(int v = e; v != s; v = path[v]){
        int u = path[v];
        rGraph[u][v] -= min_flow;
        rGraph[v][u] += min_flow;
    }
}

int ford_fulkerson(int ** rGraph, int ** fGraph ,int * path, int s, int e, int N){
    int min_flow = 0;
    while(breadth_first(rGraph, s, e, path, N)){
        min_flow = min_flow_in_path(path, s, e, rGraph);
        update_residual_edges(path, rGraph, min_flow, s, e);
        max_flow += min_flow;
    }
    return max_flow;
}



int main() {
    const char * matrixGraphFile = "macierz.txt";

    ifstream inFile;
    inFile.open(matrixGraphFile);

    int NODE_NUMBER;
    int EDGE_NUMBER;
    int N_START;
    int N_END;

    inFile >> NODE_NUMBER;
    inFile >> EDGE_NUMBER;
    inFile >> N_START;
    inFile >> N_END;

    int ** weightMatrix = new int *[NODE_NUMBER];
    int ** residualMatrix = new int *[NODE_NUMBER];
    int ** flowMatrix = new int *[NODE_NUMBER];
    int path[NODE_NUMBER];

    for(int i = 0; i < NODE_NUMBER; i++){
        weightMatrix[i] = new int[NODE_NUMBER];
        residualMatrix[i] = new int[NODE_NUMBER];
        flowMatrix[i] = new int[NODE_NUMBER];
        path[i] = -1;
    }

    for(int i = 0; i < NODE_NUMBER; i++){
        for(int j = 0; j < NODE_NUMBER; j++){
            inFile >> weightMatrix[i][j];
            residualMatrix[i][j] = weightMatrix[i][j];
            flowMatrix[i][j] = 0;
        }
    }

    int f = ford_fulkerson(residualMatrix, flowMatrix, path, N_START, N_END, NODE_NUMBER);

    cout << "Maksymalny przeplyw: " << endl;
    cout << f;
    cout << endl << endl;
    cout << "Macierz rezydualna: " << endl;
    show_matrix(residualMatrix, NODE_NUMBER);

    return 0;
}
