#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class GraphBFS {
public:
    int V;
    vector<vector<int>> adj;

    GraphBFS(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void parallelBFS(int start) {
        vector<bool> visited(V, false);
        queue<int> q;

        visited[start] = true;
        q.push(start);

        cout << "Parallel BFS: ";

        while (!q.empty()) {
            int qSize = q.size();

            #pragma omp parallel for
            for (int i = 0; i < qSize; i++) {
                int node;
                #pragma omp critical
                {
                    if (!q.empty()) {
                        node = q.front();
                        q.pop();
                        cout << node << " ";
                    }
                }

                for (int neighbor : adj[node]) {
                    if (!visited[neighbor]) {
                        #pragma omp critical
                        {
                            if (!visited[neighbor]) {
                                visited[neighbor] = true;
                                q.push(neighbor);
                            }
                        }
                    }
                }
            }
        }
        cout << endl;
    }
};

int main() {
    int V, E;
    cout << "Enter number of vertices: ";
    cin >> V;
    GraphBFS g(V);

    cout << "Enter number of edges: ";
    cin >> E;
    cout << "Enter " << E << " edges (u v):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        g.addEdge(u, v);
    }

    int startNode;
    cout << "Enter starting node for BFS: ";
    cin >> startNode;

    g.parallelBFS(startNode);

    return 0;
}