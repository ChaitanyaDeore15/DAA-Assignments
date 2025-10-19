#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
    int weight;
    int utility;
};

int knapsack(const vector<Item>& items, int W, vector<int>& selectedItems) {
    int N = items.size();
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));
    
    for (int i = 1; i <= N; ++i) {
        for (int w = 0; w <= W; ++w) {
            if (items[i-1].weight <= w) {
                int includeItem = dp[i-1][w - items[i-1].weight] + items[i-1].utility;
                if (includeItem > dp[i-1][w]) {
                    dp[i][w] = includeItem;
                } else {
                    dp[i][w] = dp[i-1][w];
                }
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    int remainingWeight = W;
    for (int i = N; i > 0; --i) {
        if (dp[i][remainingWeight] != dp[i-1][remainingWeight]) {
            selectedItems.push_back(i-1);
            remainingWeight -= items[i-1].weight;
        }
    }

    return dp[N][W];
}

int main() {
    int N, W;
    cout << "Enter the number of items : ";
    cin >> N;
    cout << "Enter the truck capacity : ";
    cin >> W;
    
    vector<Item> items(N);
    
    cout << "Enter the weight and utility of each item:\n";
    for (int i = 0; i < N; ++i) {
        cout << "Item " << i + 1 << " - Weight: ";
        cin >> items[i].weight;
        cout << "Item " << i + 1 << " - Utility: ";
        cin >> items[i].utility;
    }
    
    vector<int> selectedItems;
    
    int maxUtility = knapsack(items, W, selectedItems);
    
    cout << "Maximum utility that can be carried: " << maxUtility << endl;
    
    cout << "Items chosen :\n";
    for (int i : selectedItems) {
        cout << "Item " << i + 1 << " - Weight: " << items[i].weight << " Utility: " << items[i].utility << endl;
    }
    
    return 0;
}
