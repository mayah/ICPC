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

typedef int Vertex;
typedef int Weight;

struct Edge {
    Vertex dest;
    Weight weight;
    Edge() {}
    Edge(const Vertex& d, Weight w) : dest(d), weight(w) {}
};

typedef vector<Edge> Edges;
typedef map<Vertex, Edges> Graph;

bool step(Graph& g, int u, vector<int>& matchTo, vector<bool>& visited) {
    if (u < 0) { return true; }

    for (Edges::iterator e = g[u].begin(); e != g[u].end(); ++e) {
        if (visited[e->dest]) { continue; }
        visited[e->dest] = true;
        if (step(g, matchTo[e->dest], matchTo, visited)) {
            matchTo[u] = e->dest;
            matchTo[e->dest] = u;
            return true;
        }
    }
    return false;
}

// 0..L-1 左頂点 L..g.size()-1 右頂点
int bipartiteMatching(Graph& g, int L, int n, vector<pair<int, int> >& matching) {
    vector<int> matchTo(n, -1);
    int match = 0;

    for (int u = 0; u < L; ++u) {
        vector<bool> visited(n);
        if (step(g, u, matchTo, visited)) { ++match; }
    }

    for (int u = 0; u < L; ++u) {
        if (matchTo[u] >= 0) {
            matching.push_back(make_pair(u, matchTo[u]));
        }
    }

    return match;
}

struct HorizontalLine {
    HorizontalLine(int xBegin, int xEnd, int y) : xBegin(xBegin), xEnd(xEnd), y(y) {}
    int xBegin, xEnd, y;

    friend bool operator<(const HorizontalLine& lhs, const HorizontalLine& rhs) {
        if (lhs.xBegin != rhs.xBegin)
            return lhs.xBegin < rhs.xBegin;
        if (lhs.y != rhs.y)
            return lhs.y < rhs.y;
        return lhs.xEnd < rhs.xEnd;
    }
};

struct VerticalLine {
    VerticalLine(int x, int yBegin, int yEnd) : x(x), yBegin(yBegin), yEnd(yEnd) {}
    int x, yBegin, yEnd;

    friend bool operator<(const VerticalLine& lhs, const VerticalLine& rhs) {
        if (lhs.x != rhs.x)
            return lhs.x < rhs.x;
        if (lhs.yBegin != rhs.yBegin)
            return lhs.yBegin < rhs.yBegin;
        return lhs.yEnd < rhs.yEnd;
    }
};

int findHorizontalLineEnd(int x, int y, const vvi& field)
{
    int xx = x;
    while (true) {
        if (field[y][xx] && field[y+1][xx]) {
            xx += 1;
            continue;
        }

        if ((!field[y][xx] && field[y+1][xx]) || (field[y][xx] && !field[y+1][xx]))
            return xx;
        return -1;
    }
}

int findVerticalLineEnd(int x, int y, const vvi& field)
{
    int yy = y;
    while (true) {
        if (field[yy][x] && field[yy][x+1]) {
            yy += 1;
            continue;
        }

        if ((!field[yy][x] && field[yy][x+1]) || (field[yy][x] && !field[yy][x+1]))
            return yy;
        return -1;
    }
}

inline bool linesConflict(const HorizontalLine& hl, const VerticalLine& vl)
{
    return hl.xBegin <= vl.x && vl.x <= hl.xEnd && vl.yBegin <= hl.y && hl.y <= vl.yEnd;
}

int countCorners(int H, int W, const vvi& field)
{
    int result = 0;
    for (int y = 1; y <= H; ++y) {
        for (int x = 1; x <= W; ++x) {
            int cnt = field[y][x] + field[y][x+1] + field[y+1][x] + field[y+1][x+1];
            if (cnt == 3)
                ++result;
        }
    }

    return result;
}

int solve(int H, int W, const vvi& field)
{
    vector<HorizontalLine> horizontals;
    vector<VerticalLine> verticals;

    // collect horizontal line
    for (int y = 1; y <= H; ++y) {
        for (int x = 1; x <= W; ++x) {
            if ((!field[y][x] && field[y][x+1] &&  field[y+1][x] && field[y+1][x+1]) ||
                ( field[y][x] && field[y][x+1] && !field[y+1][x] && field[y+1][x+1])) {
                int horizontalLineEnd = findHorizontalLineEnd(x + 1, y, field);
                if (horizontalLineEnd > 0)
                    horizontals.push_back(HorizontalLine(x + 1, horizontalLineEnd, y + 1));
            }

            if ((!field[y][x] &&  field[y][x+1] && field[y+1][x] && field[y+1][x+1]) ||
                ( field[y][x] && !field[y][x+1] && field[y+1][x] && field[y+1][x+1])) {
                int verticalLineEnd = findVerticalLineEnd(x, y + 1, field);
                if (verticalLineEnd > 0)
                    verticals.push_back(VerticalLine(x + 1, y + 1, verticalLineEnd));
            }                
        }
    }

#if 0
    cout << "Horizontals:" << endl;
    for (int i = 0; i < horizontals.size(); ++i) {
        cout << "(" << horizontals[i].xBegin << "," << horizontals[i].y << ")-(" << horizontals[i].xEnd << "," << horizontals[i].y << ")" << endl;
    }

    cout << "Verticals:" << endl;
    for (int i = 0; i < verticals.size(); ++i) {
        cout << "(" << verticals[i].x << "," << verticals[i].yBegin << ")-(" << verticals[i].x << "," << verticals[i].yEnd << ")" << endl;
    }
#endif

    Graph g;
    for (int h = 0; h < horizontals.size(); ++h) {
        for (int v = 0; v < verticals.size(); ++v) {
            if (linesConflict(horizontals[h], verticals[v])) {
                g[h].push_back(Edge(v + horizontals.size(), 1));
                g[v + horizontals.size()].push_back(Edge(h, 1));
            }
        }
    }

    vector<pair<int, int> > matching;
    bipartiteMatching(g, horizontals.size(), horizontals.size() + verticals.size(), matching);

#if 0
    cout << "*** " 
         << countCorners(H, W, field) << " "
         << horizontals.size() << " "
         << verticals.size() << " "
         << matching.size() << endl;
#endif

    return 1 + countCorners(H, W, field) - horizontals.size() - verticals.size() + matching.size();
}

int main(void)
{
    for (int H, W; cin >> H >> W, (H || W); ) {
        vvi field(H + 2, vi(W + 2));
        for (int y = 0; y < H; ++y) {
            for (int x = 0; x < W; ++x) {
                char c; cin >> c;
                if (c == '#')
                    field[y+1][x+1] = 1;
            }
        }

        cout << solve(H, W, field) << endl;
    }
    return 0;
}

