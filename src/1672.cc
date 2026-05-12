#include <bits/stdc++.h>
using namespace std;
using ull = unsigned long long;

int main() {
    array<ull, 3> primes;
    ull K;
    for (int i = 0; i < 3; ++i) {
        cin >> primes[i];
    }
    cin >> K;
    priority_queue<ull, vector<ull>, greater<ull>> min_heap;
    unordered_set<ull> visited_set;
    min_heap.push(1);
    visited_set.insert(1);
    ull curr_val;
    ull next_val;
    for (int i = 0; i < K; ++i) {
        curr_val = min_heap.top();
        min_heap.pop();
        for (auto p : primes) {
            next_val = curr_val * p;
            if (visited_set.find(next_val) == visited_set.end()) {
                visited_set.insert(next_val);
                min_heap.push(next_val);
            }
        }
    }
    cout << curr_val << endl;
    return 0;
}