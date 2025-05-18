#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int MAXN = 1000;
int graph[MAXN][MAXN];
int n = 5; // Default, can be overridden

// Dijkstra algorithm
vector<int> dijkstra(int start, int end) {
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    dist[start] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    vector<int> path;
    if (dist[end] == INT_MAX) return path;

    for (int v = end; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());
    return path;
}

// A* placeholder (same as Dijkstra for now)
vector<int> a_star(int start, int end) {
    return dijkstra(start, end);
}
#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int MAXN = 1000;
int graph[MAXN][MAXN];
int n = 5; // Default, can be overridden

// Dijkstra algorithm
vector<int> dijkstra(int start, int end) {
    cout << "Running Dijkstra's algorithm..." << endl;
    vector<int> dist(n, INT_MAX);
    vector<int> parent(n, -1);
    vector<bool> visited(n, false);

    dist[start] = 0;
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
    pq.push({0, start});

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        for (int v = 0; v < n; v++) {
            if (graph[u][v] > 0 && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }

    vector<int> path;
    if (dist[end] == INT_MAX) {
        cout << "No path found from " << start << " to " << end << endl;
        return path;
    }

    for (int v = end; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());
    cout << "Path found: ";
    for (int node : path) cout << node << " ";
    cout << endl;
    return path;
}

// A* placeholder (same as Dijkstra for now)
vector<int> a_star(int start, int end) {
    cout << "Running A* algorithm..." << endl;
    return dijkstra(start, end);
}

// Traveling Salesman Problem using Bitmask DP
vector<int> tsp(const vector<vector<int>>& dist) {
    cout << "Running Traveling Salesman Problem algorithm..." << endl;
    int sz = dist.size();
    vector<vector<int>> dp(1 << sz, vector<int>(sz, INT_MAX));
    vector<vector<int>> parent(1 << sz, vector<int>(sz, -1));

    dp[1][0] = 0;

    for (int mask = 1; mask < (1 << sz); mask += 2) {
        for (int u = 0; u < sz; ++u) {
            if (!(mask & (1 << u))) continue;
            for (int v = 0; v < sz; ++v) {
                if (mask & (1 << v) || dist[u][v] == 0) continue;
                int next_mask = mask | (1 << v);
                if (dp[mask][u] + dist[u][v] < dp[next_mask][v]) {
                    dp[next_mask][v] = dp[mask][u] + dist[u][v];
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    int last = -1;
    int best = INT_MAX;
    int full_mask = (1 << sz) - 1;

    for (int i = 1; i < sz; ++i) {
        if (dp[full_mask][i] + dist[i][0] < best) {
            best = dp[full_mask][i] + dist[i][0];
            last = i;
        }
    }

    if (best == INT_MAX) {
        cout << "No path found for Traveling Salesman Problem" << endl;
        return {};
    }

    vector<int> path;
    int mask = full_mask;
    while (last != -1) {
        path.push_back(last);
        int temp = parent[mask][last];
        mask ^= (1 << last);
        last = temp;
    }
    path.push_back(0);
    reverse(path.begin(), path.end());
    cout << "Path found for Traveling Salesman Problem: ";
    for (int node : path) cout << node << " ";
    cout << endl;
    return path;
}
// Traveling Salesman Problem using Bitmask DP
vector<int> tsp(const vector<vector<int>>& dist) {
    int sz = dist.size();
    vector<vector<int>> dp(1 << sz, vector<int>(sz, INT_MAX));
    vector<vector<int>> parent(1 << sz, vector<int>(sz, -1));

    dp[1][0] = 0;

    for (int mask = 1; mask < (1 << sz); mask += 2) {
        for (int u = 0; u < sz; ++u) {
            if (!(mask & (1 << u))) continue;
            for (int v = 0; v < sz; ++v) {
                if (mask & (1 << v) || dist[u][v] == 0) continue;
                int next_mask = mask | (1 << v);
                if (dp[mask][u] + dist[u][v] < dp[next_mask][v]) {
                    dp[next_mask][v] = dp[mask][u] + dist[u][v];
                    parent[next_mask][v] = u;
                }
            }
        }
    }

    int last = -1;
    int best = INT_MAX;
    int full_mask = (1 << sz) - 1;

    for (int i = 1; i < sz; ++i) {
        if (dp[full_mask][i] + dist[i][0] < best) {
            best = dp[full_mask][i] + dist[i][0];
            last = i;
        }
    }

    if (best == INT_MAX) return {};

    vector<int> path;
    int mask = full_mask;
    while (last != -1) {
        path.push_back(last);
        int temp = parent[mask][last];
        mask ^= (1 << last);
        last = temp;
    }
    path.push_back(0);
    reverse(path.begin(), path.end());
    return path;
}
