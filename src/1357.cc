#include <bits/stdc++.h>
using namespace std;

bool is_covering(int i, int j, const vector<int> &a, int n)
{
    if (a[j] % a[i] != 0)
        return false;
    for (int k = 0; k < n; k++)
    {
        if (k == i || k == j)
            continue;
        if (a[k] % a[i] == 0 && a[j] % a[k] == 0)
        {
            return false;
        }
    }
    return true;
}

bool has_glb(int i, int j, const vector<int> &a, int n)
{
    vector<int> lower_bounds;
    for (int x : a)
    {
        if (a[i] % x == 0 && a[j] % x == 0)
        {
            lower_bounds.push_back(x);
        }
    }
    if (lower_bounds.empty())
        return false;

    for (int candidate : lower_bounds)
    {
        bool is_glb = true;
        for (int other : lower_bounds)
        {
            if (candidate % other != 0)
            {
                is_glb = false;
                break;
            }
        }
        if (is_glb)
            return true;
    }
    return false;
}

bool has_lub(int i, int j, const vector<int> &a, int n)
{
    vector<int> upper_bounds;
    for (int x : a)
    {
        if (x % a[i] == 0 && x % a[j] == 0)
        {
            upper_bounds.push_back(x);
        }
    }
    if (upper_bounds.empty())
        return false;

    for (int candidate : upper_bounds)
    {
        bool is_lub = true;
        for (int other : upper_bounds)
        {
            if (other % candidate != 0)
            {
                is_lub = false;
                break;
            }
        }
        if (is_lub)
            return true;
    }
    return false;
}

int main() {
    int n;
    if (!(cin >> n)){
        return 0;
    }
    vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    bool first = true;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                continue;
            }
            if (i < j && is_covering(i, j, a, n))
            {
                if (!first)
                    cout << " ";
                cout << "(" << a[i] << "," << a[j] << ")";
                first = false;
            }
        }
    }
    cout << endl;

    bool is_lattice = true;
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (!has_glb(i, j, a, n) || !has_lub(i, j, a, n))
            {
                is_lattice = false;
                break;
            }
        }
        if (!is_lattice)
            break;
    }
    if (is_lattice)
        {cout << "YES" << endl;}
    else
        {cout << "NO" << endl;}
    return 0;
}