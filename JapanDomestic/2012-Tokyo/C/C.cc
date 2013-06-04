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

class Dice {
public:
    int dice[3]; // top, front, right

    Dice(int top, int front) {
        dice[0] = top;
        dice[1] = front;
        dice[2] = calcRight(top, front);
    }

    int calcRight(int top, int front) {
        static int rights[6][6] = {
            { 0, 3, 5, 2, 4, 0 },
            { 4, 0, 1, 6, 0, 3 },
            { 2, 6, 0, 0, 1, 5 },
            { 5, 1, 0, 0, 6, 2 },
            { 3, 0, 6, 1, 0, 4 },
            { 0, 4, 2, 5, 3, 0 }
        };

        return rights[top - 1][front - 1];
    }

    int top() { return dice[0]; }
    int front() { return dice[1]; }
    int right() { return dice[2]; }
    int bottom() { return 7 - top(); }
    int back() { return 7 - front(); }
    int left() { return 7 - right(); }

    Dice rollBack() {
        return Dice(front(), bottom());
    }
    Dice rollFront() {
        return Dice(back(), top());
    }
    Dice rollLeft() {
        return Dice(right(), front());
    }
    Dice rollRight() {
        return Dice(left(), front());
    }
};

void rollDice(map<pair<int, int>, int>& heightMap,
              map<pair<int, int>, int>& topMap,
              int top, int front)
{
    int x = 0, y = 0;
    Dice d(top, front);

    while (true) {
        int currentHeight = heightMap[make_pair(x, y)];

        pair<int, int> vs[] = {
            make_pair(d.back(), 0),
            make_pair(d.front(), 1),
            make_pair(d.right(), 2),
            make_pair(d.left(), 3),
        };

        sort(vs, vs + 4, greater<pair<int, int> >());
        
        bool rolled = false;
        for (int i = 0; !rolled && i < 4; ++i) {
            switch (vs[i].second) {
            case 0:
                if (d.back() >= 4 && heightMap[make_pair(x, y + 1)] < currentHeight) {
                    d = d.rollBack();
                    y += 1;
                    rolled = true;
                }
                break;
            case 1:
                if (d.front() >= 4 && heightMap[make_pair(x, y - 1)] < currentHeight) { 
                    d = d.rollFront();
                    y -= 1;
                    rolled = true;
                }
                break;
            case 2:
                // rotate right?
                if (d.right() >= 4 && heightMap[make_pair(x + 1, y)] < currentHeight) {
                    d = d.rollRight();
                    x += 1;
                    rolled = true;
                }
                break;
            case 3:
                if (d.left() >= 4 && heightMap[make_pair(x - 1, y)] < currentHeight) {
                    d = d.rollLeft();
                    x -= 1;
                    rolled = true;
                }
                break;
            }
        }
        
        if (!rolled) {
            heightMap[make_pair(x, y)] += 1;
            topMap[make_pair(x, y)] = d.top();
            return;
        }
    }
}

int main(void)
{
    for (int n; cin >> n, n; ) {
        map<pair<int, int>, int> heightMap;
        map<pair<int, int>, int> topMap;

        for (int i = 0; i < n; ++i) {
            int t, f; cin >> t >> f;
            rollDice(heightMap, topMap, t, f);
        }

        int dices[7] = { 0 };

        for (map<pair<int, int>, int>::iterator it = topMap.begin(); it != topMap.end(); ++it) {
            dices[it->second]++;
        }

        cout << dices[1] << ' '
             << dices[2] << ' '
             << dices[3] << ' '
             << dices[4] << ' '
             << dices[5] << ' '
             << dices[6] << endl;
    }

    return 0;
}

