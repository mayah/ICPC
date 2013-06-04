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
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;

// 解答の方針
// 各鉄道会社だけで、floyd で駅間の距離を求めてて運賃表を作る。
// その運賃表を全てマージ。
// 最後にもういちど floyd

const int INF = 1 << 29;

void floyd(vvi& m, int N)
{
    for (int k = 0; k < N; ++k) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                m[i][j] = min(m[i][j], m[i][k] + m[k][j]);
            }
        }
    }
}

int fareFromDistance(int distance, const vi& qs, const vi& rs)
{
    int fare = min(distance, qs[0]) * rs[0];
    for (int i = 0; qs[i] < distance; ++i)
        fare += (min(distance, qs[i + 1]) - qs[i]) * rs[i + 1];
    return fare;
}

void convertDistanceToFare(vvi& m, int N, const vi& qs, const vi& rs)
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (m[i][j] == INF)
                continue;
            m[i][j] = fareFromDistance(m[i][j], qs, rs);
        }
    }
}

int main(void)
{
    for (int N, M, C, S, G; cin >> N >> M >> C >> S >> G, N; ) {
        vvvi matrixByCompany(C, vvi(N, vi(N, INF)));
        for (int i = 0; i < M; ++i) {
            int x, y, d, c;
            cin >> x >> y >> d >> c;
            matrixByCompany[c-1][x-1][y-1] = min(d, matrixByCompany[c-1][x-1][y-1]);
            matrixByCompany[c-1][y-1][x-1] = min(d, matrixByCompany[c-1][y-1][x-1]);
        }

        for (int i = 0; i < C; ++i)
            floyd(matrixByCompany[i], N);

        vi ps(C);
        for (int i = 0; i < C; ++i) {
            cin >> ps[i];
        }

        for (int i = 0; i < C; ++i) {
            vi qs(ps[i]), rs(ps[i]);
            for (int j = 0; j < ps[i] - 1; ++j)
                cin >> qs[j];
            qs[ps[i] - 1] = INF;
            for (int j = 0; j < ps[i]; ++j)
                cin >> rs[j];

            convertDistanceToFare(matrixByCompany[i], N, qs, rs);
        }

        vvi fare(N, vi(N, INF));
        for (int c = 0; c < C; ++c) {
            for (int i = 0; i < N; ++i) {
                for (int j = 0; j < N; ++j) {
                    fare[i][j] = min(fare[i][j], matrixByCompany[c][i][j]);
                }
            }
        }

        floyd(fare, N);

        if (fare[S-1][G-1] >= INF)
            cout << -1 << endl;
        else
            cout << fare[S-1][G-1] << endl;
    }
}
