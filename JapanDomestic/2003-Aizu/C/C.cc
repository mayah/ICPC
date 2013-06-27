#include <iostream>
#include <vector>
#include <cctype>

using namespace std;

string solve(const vector<string>& field, int H, int W)
{
    vector<vector<string> > work(H + 1, vector<string>(W + 1));
    
    for (int h = H - 1; h >= 0; --h) {
        for (int w = W - 1; w >= 0; --w) {
            if (!isdigit(field[h][w]))
                continue;

            // 下と右のうち、長い方を選択
            if ((work[h][w + 1].size() > work[h + 1][w].size()) ||
                ((work[h][w + 1].size() == work[h + 1][w].size()) &&
                 work[h][w + 1] > work[h + 1][w])) {
                work[h][w] = field[h][w] + work[h][w + 1];
            } else {
                work[h][w] = field[h][w] + work[h + 1][w];
            }
        }
    }

    // 全て計算し終わったので、最も大きいものを探す。このときは先頭の 0 は取り除く。
    string result;
    for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
            if (work[h][w].find_first_not_of('0') != string::npos)
                work[h][w] = work[h][w].substr(work[h][w].find_first_not_of('0'));
            if (work[h][w].size() > result.size() || (work[h][w].size() == result.size() && work[h][w] > result))
                result = work[h][w];
        }
    }
    
    return result;
}

int main(void)
{
	int W, H;
	while (cin >> W >> H, (W || H)) {
        // 番兵として外部に 'X' を入れておく。
        vector<string> field(H + 1);
        for (int h = 0; h < H; ++h) {
            cin >> field[h];
            field[h] += 'X';
        }
        field[H] = string(W + 1, 'X');

        cout << solve(field, H, W) << endl;
    }
}

