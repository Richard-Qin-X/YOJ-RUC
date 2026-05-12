#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll fast_pow(ll base, ll power, ll mod) {
    ll res = 1;
    while (power > 0)
    {
        if (power % 2 == 1) {
            res = (res * base) % mod;
        }
        base = (base * base) % mod;
        power /= 2;
    }
    return res;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    ll x;
    ll p;
    cin >> x >> p;
    ll ans = fast_pow(x, p-2, p);
    cout << ans << endl;
    return 0;
}