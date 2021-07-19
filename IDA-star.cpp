//POJ - 1107 47MS, HDU - 1043 1092MS
#include <cstdio>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <map>
#include <cstdlib>
#include <cmath>
using namespace std;

typedef long long LL;
const int N = 4e5+7;
const int M = 1e6 + 7;

const int inf = 0x3f3f3f3f;
const int Mod = 1e6 + 3;
// 1 2 3 7 8 3 6 9

int factor[10];

// d u r l
int DIR[4] = {3, -3, 1, -1};
char DIR_C[4] = {'d', 'u', 'r', 'l'};
int dig[10];
int hash_map[M];

void tileToArray(int num) {
    for (int i = 8; i >= 0; --i) {
        dig[i] = num % 10;
        num /= 10;
    }
}

int ArrayToNum(int *dig) {
    int num = 0;
    for(int i = 0; i < 9; ++i) {
        num = num * 10 + dig[i];
    }
    return num;
}

int status(int tile, int dir = -2) {
    if (dir != -2)
       hash_map[tile % Mod] = dir;
    return hash_map[tile % Mod];
}

int h(int *tile = dig) {
    int res = 0;
    for (int i = 0; i < 9; ++i) {
        int v = tile[i] - 1;
        int x1 = i / 3, y1 = i % 3;
        int x2 = v / 3, y2 = v % 3;
        res += abs(x1 - x2) + abs(y1 - y2);
    }
    return res;
}

bool dfs(int tile, int pos, int depth, int maxDepth, int end, string& res) {
    if (tile == end) {
        return true;
    }
    tileToArray(tile);
    if (depth > maxDepth)
        return false;
    for (int d = 0; d < 4; ++d) {
        int x1 = pos / 3 + DIR[d] / 3, y1 = pos % 3 + DIR[d] % 3;
        if (x1 >= 0 && x1 < 3 && y1 >= 0 && y1 < 3) {
            int newPos = x1 * 3 + y1;
            swap(dig[pos], dig[newPos]);
            int newTile = ArrayToNum(dig);
            if (status(newTile) >= 0){
                swap(dig[pos], dig[newPos]);
                continue;
            }
            int measure = h() + depth + 1;
            if (measure <= maxDepth) {
                status(newTile, d);
                if (dfs(newTile, newPos, depth + 1, maxDepth, end, res)) {
                    res = DIR_C[status(newTile)] + res;
                    swap(dig[pos], dig[newPos]);
                    status(newTile, -1);
                    return true;
                }
                status(newTile, -1);
            }
            swap(dig[pos], dig[newPos]);
        }
    }
    return false;
}

string idaStar(int start, int end) {
    int maxDepth = 2;
    while(1) {
        string res = "";
        tileToArray(start);
        int pos = 0;
        for (int i = 0; i < 9; ++i) {
            if(dig[i] == 9) {
                pos = i;
                break;
            }
        }
        if (dfs(start, pos, 1, maxDepth, end, res)) {
            return res;
        }
        maxDepth++;
    }
}

int resPair(int *input = dig) {
    int resp = 0;
    for (int i = 0; i < 9; ++i) {
        for(int j = i + 1; j < 9; ++j) {
            if (input[i] == 9 || input[j] == 9 )
                continue;
            if (input[i] > input[j]) {
                ++resp;
            }
        }
    }
    return resp;
}

bool isChangeable() {
    int endOrder[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int r1 = resPair(), r2 = resPair(endOrder);
    return r1 % 2 == r2 % 2;
}

void Init() {
    memset(hash_map, -1, sizeof hash_map);
}

int main() {
#ifdef LOCAL
    freopen("in", "r", stdin);
    clock_t start = clock();
#endif
    Init();
//    debug();
    while(true) {
        bool finish = false;
        for(int i=0;i<9;i++) {
            string tile;
            if(!(cin >> tile)) {
                finish = true;
                break;
            }
            dig[i] = (tile[0] == 'x') ? 9 : tile[0] - '0';
        }
        if (finish)
            break;
        if (!isChangeable()) {
            puts("unsolvable");
            continue;
        }

        int start_index = ArrayToNum(dig);
        int end_index = 123456789;

        string road = idaStar(start_index, end_index);

//        printf("%d : %s\n", start_index, road.c_str());
        printf("%s\n", road.c_str());
    }

#ifdef LOCAL
    clock_t end = clock();
    double times = end - start;
    times /= CLOCKS_PER_SEC;
    cout << "runtime " << times << " second" << endl;
#endif
}