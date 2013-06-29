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

typedef vector<int> vi;
typedef vector<vi> vvi;

struct P {
    int x;
    int y;

    P(int x, int y) : x(x), y(y) {}

    P operator+(const P& p) { return P(x + p.x, y + p.y); }
    P operator-(const P& p) { return P(x - p.x, y - p.y); }
    friend bool operator==(const P& lhs, const P& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
    friend bool operator<(const P& lhs, const P& rhs) { return make_pair(lhs.x, lhs.y) < make_pair(rhs.x, rhs.y); }
};

struct State {
	State() : king(-1, -1), space1(-1, -1), space2(-1, -1) {}
    State(P king, P space1, P space2) : king(king), space1(space1), space2(space2) {}

    friend bool operator<(const State& lhs, const State& rhs) {
        if (!(lhs.king == rhs.king))
            return lhs.king < rhs.king;
        if (!(lhs.space1 == rhs.space1))
            return lhs.space1 < rhs.space1;
        return lhs.space2 < rhs.space2;
    }

    friend bool operator>(const State& lhs, const State& rhs) {
        return rhs < lhs;
    }

	P king;
	P space1, space2;
};

struct StateAndCost {
	StateAndCost(const State& state, int cost) : dest(state), weight(cost) {}

	State dest;
	int weight;
};

typedef int Weight;
typedef State Vertex;
typedef StateAndCost Edge;
typedef vector<Edge> Edges;
typedef map<State, int> Potential;

// for MST / Dijkstra
bool operator>(const Edge& lhs, const Edge& rhs) {
    if (lhs.weight != rhs.weight) { return lhs.weight > rhs.weight; }
    return lhs.dest > rhs.dest;
}

int DY[] = { -1, 0, 1,  0 };
int DX[] = {  0, 1, 0, -1 };
P NEXT_SPACE[][2] = {
    { P(0, -1), P( 1, -1) },
    { P(2,  0), P( 2,  1) },
    { P(0,  2), P( 1,  2) },
    { P(-1, 0), P(-1,  1) },
};
P PREV_SPACE[][2] = {
    { P(0, 1), P(1, 1) },
    { P(0, 0), P(0, 1) },
    { P(0, 0), P(1, 0) },
    { P(1, 0), P(1, 1) },
};

int calculateMoveCost(const vector<vector<char> >& field, P king, P s, P t)
{
    const int INF = 1 << 28;
    vvi costs(field.size(), vi(field[0].size(), INF));
    costs[king.y + 0][king.x + 0] = -1;
    costs[king.y + 0][king.x + 1] = -1;
    costs[king.y + 1][king.x + 0] = -1;
    costs[king.y + 1][king.x + 1] = -1;

    queue<P> Q;
    Q.push(s);
    costs[s.y][s.x] = 0;

    while (!Q.empty()) {
        P p = Q.front(); Q.pop();
        int x = p.x, y = p.y;

        if (t.x == x && t.y == y)
            return costs[t.y][t.x];

        for (int i = 0; i < 4; ++i) {
            int xx = x + DX[i];
            int yy = y + DY[i];
            if (yy < 0 || field.size() <= y || xx < 0 && field[yy].size() <= xx)
                continue;

            if (field[yy][xx] != '.')
                continue;

            if (costs[yy][xx] < INF)
                continue;

            costs[yy][xx] = costs[y][x] + 1;
            Q.push(P(xx, yy));
        }        
    }

    return -1;
}

int calculateMoveCost(const vector<vector<char> >& field, P king, P s1, P s2, P t1, P t2)
{
    // s1 -> t1, s2 -> t2
    int cost11 = calculateMoveCost(field, king, s1, t1);
    int cost12 = calculateMoveCost(field, king, s2, t2);
    int cost1 = -1;
    if (cost11 >= 0 && cost12 >= 0)
        cost1 = cost11 + cost12;

    int cost21 = calculateMoveCost(field, king, s1, t2);
    int cost22 = calculateMoveCost(field, king, s2, t1);
    int cost2 = -1;
    if (cost21 >= 0 && cost22 >= 0)
        cost2 = cost21 + cost22;

    if (cost1 < 0)
        return cost2;
    if (cost2 < 0)
        return cost1;
    return min(cost1, cost2);
}

int solve(int H, int W, const vector<vector<char> >& field, const State& firstState)
{
    Potential pot;
    priority_queue<Edge, Edges, greater<Edge> > Q;

    Q.push(Edge(firstState, 0));

    while (!Q.empty()) {
        Edge edge = Q.top(); Q.pop();
        Vertex p = edge.dest;
        Weight curr = edge.weight;

        // found
        if (p.king == P(1, 1)) {
            return curr;
        }

        if (pot.count(p))
            continue; // already visited.
        pot[p] = curr;

	    // Go up
        for (int i = 0; i < 4; ++i) {
            int x = p.king.x + DX[i];
            int y = p.king.y + DY[i];
            if (field[y][x] != '.' || field[y][x+1] != '.' || field[y+1][x] != '.' || field[y+1][x+1] != '.')
                continue;

            int moveCost = calculateMoveCost(field, p.king, p.space1, p.space2, p.king + NEXT_SPACE[i][0], p.king + NEXT_SPACE[i][1]);
            if (moveCost < 0)
                continue;

            State newState(p.king + P(DX[i], DY[i]), p.king + PREV_SPACE[i][0], p.king + PREV_SPACE[i][1]);
            if (pot.count(newState))
                continue;
            Q.push(Edge(newState, curr + moveCost + 1));
        }
    }

    return -1;
}

int main(void)
{
    for (int H, W; cin >> H >> W, (H || W); ) {
    	vector<vector<char> > field(H + 2, vector<char>(W + 2, 'X'));
    	State initialState;
    	for (int y = 1; y <= H; ++y) {
    		for (int x = 1; x <= W; ++x) {
    			cin >> field[y][x];
    			if (field[y][x] == 'X') {
                    if (initialState.king == P(-1, -1))
                        initialState.king = P(x, y);
                    field[y][x] = '.';
    			} else if (field[y][x] == '.' && initialState.space1 == P(-1, -1)) {
    				initialState.space1 = P(x, y);
    			} else if (field[y][x] == '.' && initialState.space2 == P(-1, -1)) {
    				initialState.space2 = P(x, y);
    			} else if (field[y][x] == 'o') {
    				field[y][x] = '.'; // We don't want to distinguish '.' from 'o' here.
    			}
    		}
    	}

    	cout << solve(H, W, field, initialState) << endl;
    }
    return 0;
}

