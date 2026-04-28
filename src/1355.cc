#include <bits/stdc++.h>
using namespace std;
using ll = long long;

struct Intersection
{
    ll a;
    ll b;
};


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n;
    cin >> n;
    vector<Intersection> intersections(n);
    for (int i = 0; i < n; ++i) {
        cin >> intersections[i].a >> intersections[i].b;
    }
    sort(intersections.begin(), intersections.end(), [](const Intersection& i1, const Intersection& i2) {
        return i1.a < i2.a;
    });
    ll start = intersections[0].a;
    ll end = intersections[0].b;
    ll total = 0;
    ll next_start;
    ll next_end;
    for (int i = 1; i < n; ++i) {
        next_start = intersections[i].a;
        next_end = intersections[i].b;
        if (next_start <= end)
        {
            end = max(end, next_end);
        }
        else {
            total += end - start;
            start = next_start;
            end = next_end;
        }
    }
    total += end - start;
    cout << total << endl;
    return 0;
}