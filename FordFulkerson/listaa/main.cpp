#include <iostream>
#include <iomanip>
#include <fstream>
#include <queue>
#include "limits.h"

using namespace std;

int max_flow = 0;

void show_matrix(int ** arr, int row, int col){
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            cout << arr[i][j];
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

bool breadth_first(int **rList, int s, int e, int *path, int EDGE_NUM, int NODE_NUM) {
    bool visited[NODE_NUM];
    for(int i = 0; i < NODE_NUM; i++){
        visited[i] = false;
    }

    queue<int> q;
    q.push(s);
    visited[s] = true;
    path[s] = -1;

    while(!q.empty()){
        int u = q.front();
        q.pop();

        for(int i = 0; i < EDGE_NUM; i++){

            if(rList[i][0] == u){
                if(visited[rList[i][1]] == false && rList[i][2] > 0){
                    q.push(rList[i][1]);
                    path[rList[i][1]] = u;
                    visited[rList[i][1]] = true;
                }
            }
            if(rList[i][1] == u){
                if(visited[rList[i][0]] == false && rList[i][3] > 0){
                    q.push(rList[i][0]);
                    path[rList[i][0]] = u;
                    visited[rList[i][1]] = true;
                }
            }

        }
    }
    return (visited[e] == true);
}

int min_flow_in_path(int *path, int s, int e, int **rList, int EDGE_NUM) {
    int minF = INT_MAX;
    for(int v = e; v != s; v = path[v]){
        int u = path[v];
        for(int i = 0; i < EDGE_NUM; i++) {
            if(u == rList[i][0] && v == rList[i][1]){
                minF = min(minF, rList[i][2]);
            }
            if(u == rList[i][1] && v == rList[i][0]){
                minF = min(minF, rList[i][3]);
            }
        }
    }
    return minF;
}

void update_residual_edges(int *path, int **rList, int min_flow, int s, int e, int EDGE_NUM) {
    for(int v = e; v != s; v = path[v]){
        int u = path[v];
        for(int i = 0; i < EDGE_NUM; i++) {
            if(u == rList[i][0] && v == rList[i][1]){
                rList[i][2] -= min_flow;
                rList[i][3] += min_flow;
            }
            if(u == rList[i][1] && v == rList[i][0]){
                rList[i][2] += min_flow;
                rList[i][3] -= min_flow;
            }
        }
    }
}


int ford_fulkerson(int **rList, int **fList, int * path, int s, int e, int EDGE_NUM, int NODE_NUM) {
    int min_flow = 0;
    while(breadth_first(rList, s, e, path, EDGE_NUM, NODE_NUM)){
        min_flow = min_flow_in_path(path, s, e, rList, EDGE_NUM);
        update_residual_edges(path, rList, min_flow, s, e, EDGE_NUM);

        max_flow += min_flow;
    }
    return max_flow;
}


int main() {
    const char * listGraphFile = "lista.txt";

    ifstream inFile;
    inFile.open(listGraphFile);

    int NODE_NUMBER;
    int EDGE_NUMBER;
    int N_START;
    int N_END;

    inFile >> NODE_NUMBER;
    inFile >> EDGE_NUMBER;
    inFile >> N_START;
    inFile >> N_END;

    int ** weightList = new int *[EDGE_NUMBER];
    int ** residualList = new int *[EDGE_NUMBER];
    int ** flowList = new int *[EDGE_NUMBER];
    int path[NODE_NUMBER];

    for(int i = 0; i < EDGE_NUMBER; i++){
        weightList[i] = new int[3];
        residualList[i] = new int[4];
        flowList[i] = new int[4];
        path[i] = -1;
    }

    for(int i = 0; i < EDGE_NUMBER; i++){
        for(int j = 0; j < 4; j++){
            if(j < 3){
                inFile >> weightList[i][j];
                residualList[i][j] = weightList[i][j];
            }
            else{
                residualList[i][j] = 0;
            }
            if(j < 2){
                flowList[i][j] = weightList[i][j];
            }
            else{
                flowList[i][j] = 0;
            }
        }
    }

    int f = ford_fulkerson(residualList, flowList, path, N_START, N_END, EDGE_NUMBER, NODE_NUMBER);

    cout << "Maksymalny przeplyw: " << endl;
    cout << f<< endl<<endl;
    cout << "Macierz rezydualna: " << endl;
    show_matrix(residualList, EDGE_NUMBER, 4);

    return 0;
}

