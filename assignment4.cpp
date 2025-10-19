#include <bits/stdc++.h>
using namespace std;

struct Edge {
    int to;
    double weight; 
};

struct Node {
    int id;
    double dist;
    bool operator>(const Node& other) const { return dist > other.dist; }
};

vector<double> dijkstra(int N, int source, vector<vector<Edge>>& adj, vector<int>& parent) {
    vector<double> dist(N, 1e9);
    dist[source] = 0.0;
    parent.assign(N, -1);

    priority_queue<Node, vector<Node>, greater<Node>> pq;
    pq.push({source, 0.0});

    while (!pq.empty()) {
        Node current = pq.top(); pq.pop();
        int u = current.id;
        double d = current.dist;

        if (d > dist[u]) continue; 

        for (auto &edge : adj[u]) {
            int v = edge.to;
            double w = edge.weight;
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                parent[v] = u;
                pq.push({v, dist[v]});
            }
        }
    }
    return dist;
}

vector<int> reconstructPath(int dest, const vector<int>& parent) {
    vector<int> path;
    for (int v = dest; v != -1; v = parent[v]) path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    int N, M; 
    cout << "Enter number of intersections (nodes) and roads (edges): ";
    cin >> N >> M;

    vector<vector<Edge>> adj(N);
    cout << "Enter edges (u v weight):\n";
    for (int i = 0; i < M; i++) {
        int u, v;
        double w;
        cin >> u >> v >> w; 
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); 
    }

    int source;
    cout << "Enter ambulance starting location (source node): ";
    cin >> source;

    int H; 
    cout << "Enter number of hospitals: ";
    cin >> H;
    vector<int> hospitals(H);
    cout << "Enter hospital node indices:\n";
    for (int i = 0; i < H; i++) cin >> hospitals[i];

    vector<int> parent;
    vector<double> dist = dijkstra(N, source, adj, parent);

    double minDist = 1e9;
    int nearestHospital = -1;
    for (int h : hospitals) {
        if (dist[h] < minDist) {
            minDist = dist[h];
            nearestHospital = h;
        }
    }

    vector<int> path = reconstructPath(nearestHospital, parent);

    cout << "\nNearest hospital: Node " << nearestHospital << "\n";
    cout << "Shortest travel time: " << minDist << " minutes\n";
    cout << "Path: ";
    for (int v : path) cout << v << " ";
    cout << "\n";

    return 0;
}
