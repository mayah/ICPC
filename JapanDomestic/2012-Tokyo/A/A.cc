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

int convert(int y, int m, int d)
{
    y -= 1; m -= 1; d -= 1;
    
    int days = 0;
    days += (y / 3) * (200 + 195 + 195);
    y %= 3;

    if (y == 0 || y == 1) {
        days += 195 * y;
        for (int i = 0; i < m; ++i) {
            if (i % 2 == 0)
                days += 20;
            else
                days += 19;
        }
        days += d;
    } else if (y == 2) {
        days += 195 * 2;
        days += m * 20;
        days += d;
    }

    return days;
}

int main(void)
{
    int N; cin >> N;

    while (N--) {
        int y, m, d; cin >> y >> m >> d;
        cout << (convert(1000, 1, 1) - convert(y, m, d)) << endl;
    }
    return 0;
}

