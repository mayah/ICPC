#define _USE_MATH_DEFINES
#include <iostream>
#include <iomanip>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <utility>
#include <complex>
#include <cmath>
#include <cassert>

using namespace std;
typedef complex<double> P;
typedef vector<int> vi;
typedef vector<vector<int> > vvi;

int convert(int a, int L)
{
    vector<int> vs;
    for (int i = 0; i < L; ++i) {
        vs.push_back(a % 10);
        a /= 10;
    }

    sort(vs.begin(), vs.end());

    int minValue = 0;
    for (int i = 0; i < L; ++i)
        minValue = minValue * 10 + vs[i];

    int maxValue = 0;
    for (int i = L - 1; i >= 0; --i)
        maxValue = maxValue * 10 + vs[i];

    return maxValue - minValue;
}

void solve(int a0, int L)
{
    map<int, int> visited;
    visited[a0] = 0;
    int i = 0;

    int a = a0;
    while (true) {
        a = convert(a, L);
        ++i;

        if (visited.count(a)) {
            cout << visited[a] << ' ' << a << ' ' << (i - visited[a]) << endl;
            break;
        }

        visited[a] = i;
    }
}

int main(void)
{
    for (int a0, L; cin >> a0 >> L, (a0 || L); ) {
        solve(a0, L);
    }

    return 0;
}

