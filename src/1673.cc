#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int n, m;
    cin >> n >> m;
    map<string, string> int_to_ext;
    map<string, string> ext_to_int;
    string internal_addr, external_addr;
    for (int i = 0; i < n; ++i) {
        cin >> internal_addr >> external_addr;
        int_to_ext[internal_addr] = external_addr;
        ext_to_int[external_addr] = internal_addr;
    }
    string direction, src, dst;
    for (int i = 0; i < m; ++i) {
        cin >> direction >> src >> dst;
        if (direction == "OUT") {
            auto it = int_to_ext.find(src);
            if (it == int_to_ext.end()) {
                cout << "DROP" << "\n";
                continue;
            }
            src = it->second;
            cout << src << ' ' << dst << '\n';
        }
        else {
            auto it = ext_to_int.find(dst);
            if (it == ext_to_int.end())
            {
                cout << "DROP" << "\n";
                continue;
            }
            dst = it->second;
            cout << src << ' ' << dst << '\n';
        }
    }
    return 0;
}