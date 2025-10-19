#include <iostream>
#include <vector>
#include <limits>
using namespace std;

const double INF = 1e9;

struct Edge
{
    int to;
    double cost;
};

pair<double, vector<int>> multistageOptimalPath(
    const vector<vector<vector<Edge>>> &stages)
{

    int N = stages.size();        
    vector<vector<double>> dp(N); 
    vector<vector<int>> parent(N); 

    int stage0_size = stages[0].size();
    dp[0].resize(stage0_size, 0);
    parent[0].resize(stage0_size, -1);

    for (int i = 1; i < N; i++)
    {
        int stage_size = stages[i].size();
        dp[i].resize(stage_size, INF);
        parent[i].resize(stage_size, -1);

        for (int j = 0; j < stage_size; j++)
        {
            for (int k = 0; k < stages[i - 1].size(); k++)
            {
                for (auto &e : stages[i - 1][k])
                {
                    if (e.to == j)
                    {
                        double cost = dp[i - 1][k] + e.cost;
                        if (cost < dp[i][j])
                        {
                            dp[i][j] = cost;
                            parent[i][j] = k;
                        }
                    }
                }
            }
        }
    }

    double minCost = INF;
    int lastNode = -1;
    for (int i = 0; i < dp[N - 1].size(); i++)
    {
        if (dp[N - 1][i] < minCost)
        {
            minCost = dp[N - 1][i];
            lastNode = i;
        }
    }

    vector<int> path(N);
    int node = lastNode;
    for (int i = N - 1; i >= 0; i--)
    {
        path[i] = node;
        node = parent[i][node];
    }

    return {minCost, path};
}

int main()
{
    int stages_count;
    cout << "Enter number of stages: ";
    cin >> stages_count;

    vector<vector<vector<Edge>>> stages(stages_count);

    for (int i = 0; i < stages_count; i++)
    {
        int nodes;
        cout << "Enter number of nodes in stage " << i + 1 << ": ";
        cin >> nodes;
        stages[i].resize(nodes);

        if (i == stages_count - 1)
            continue; 

        for (int j = 0; j < nodes; j++)
        {
            int edges;
            cout << "Enter number of outgoing edges from node " << j
                 << " in stage " << i + 1 << ": ";
            cin >> edges;

            for (int k = 0; k < edges; k++)
            {
                int to;
                double cost;
                cout << "  Edge to node in next stage (0-based index) and cost: ";
                cin >> to >> cost;
                stages[i][j].push_back({to, cost});
            }
        }
    }
    auto result = multistageOptimalPath(stages);
    double minCost = result.first;
    vector<int> path = result.second;

    cout << "\nOptimal path through stages (0-based node indices): ";
    for (int i = 0; i < path.size(); i++)
    {
        cout << path[i] << " ";
    }
    cout << "\nMinimum cost: " << minCost << endl;

    return 0;
}
