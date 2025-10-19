#include <bits/stdc++.h>
using namespace std;

struct Item {
    string name;      
    double weight;    
    double utility;  
    bool divisible;   
};

bool cmp(const Item &a, const Item &b) {
    return (a.utility / a.weight) > (b.utility / b.weight);
}

double fractionalKnapsack(vector<Item> &items, double W, vector<pair<string,double>> &taken) {
    sort(items.begin(), items.end(), cmp);

    double totalUtility = 0.0;
    double remainingWeight = W;

    for (auto &item : items) {
        if (remainingWeight <= 0)
            break;

        if (item.divisible) {
            double wt = min(item.weight, remainingWeight);
            totalUtility += (wt * item.utility / item.weight);
            taken.push_back({item.name, wt});
            remainingWeight -= wt;
        } else {
            if (item.weight <= remainingWeight) {
                totalUtility += item.utility;
                taken.push_back({item.name, item.weight});
                remainingWeight -= item.weight;
            }
        }
    }

    return totalUtility;
}

int main() {
    int N;
    double W;
    cout << "Enter number of relief items: ";
    cin >> N;
    cout << "Enter boat weight capacity (kg): ";
    cin >> W;

    vector<Item> items(N);
    cout << "Enter item details (Name Weight Utility Divisible[0/1]):\n";
    for (int i = 0; i < N; ++i) {
        cin >> items[i].name >> items[i].weight >> items[i].utility >> items[i].divisible;
    }

    vector<pair<string,double>> takenItems;
    double maxUtility = fractionalKnapsack(items, W, takenItems);

    cout << fixed << setprecision(2);
    cout << "\nMaximum utility value that can be transported: " << maxUtility << "\n";
    cout << "Items taken:\n";
    for (auto &p : takenItems) {
        cout << p.first << " - Weight taken: " << p.second << " kg\n";
    }

    return 0;
}
