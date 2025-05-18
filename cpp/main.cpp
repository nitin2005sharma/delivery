#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "algorithms.cpp"

using namespace std;

// Example graph initialization (hardcoded here)
void init_graph() {
    // Example weighted adjacency matrix (symmetric)
    // 0  2  9  0  0
    // 1  0  6  4  0
    // 0  7  0  8  2
    // 0  0  0  0  3
    // 0  0  0  0  0
    int temp_graph[5][5] = {
        {0, 2, 9, 0, 0},
        {1, 0, 6, 4, 0},
        {0, 7, 0, 8, 2},
        {0, 0, 0, 0, 3},
        {0, 0, 0, 0, 0}
    };
    for(int i=0; i<5; i++)
        for(int j=0; j<5; j++)
            graph[i][j] = temp_graph[i][j];
    n = 5;
}

int main(int argc, char* argv[]) {
    if(argc < 4) {
        cout << "Usage: ./main_exec <algorithm> <start> <end>\n";
        return 1;
    }
    string algo = argv[1];
    int start = stoi(argv[2]);
    int end = stoi(argv[3]);

    init_graph();

    vector<int> path;

    if(algo == "dijkstra") {
        path = dijkstra(start, end);
    } else if(algo == "astar") {
        path = a_star(start, end);
    } else if(algo == "tsp") {
        vector<vector<int>> dist(n, vector<int>(n, 0));
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                dist[i][j] = graph[i][j];
            }
        }
        path = tsp(dist);
    } else {
        cout << "Unknown algorithm\n";
        return 1;
    }

    for(int node : path) {
        cout << node << " ";
    }
    cout << "\n";

    return 0;
}
