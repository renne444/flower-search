//A* search  POJ - 1107 73ms, HDU - 1043 1482MS
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

const int inf = 0x3f3f3f3f;
// 1 2 3 7 8 3 6 9

int factor[10];

// d u r l
int DIR[4] = {3, -3, 1, -1};
char DIR_C[4] = {'d', 'u', 'r', 'l'};

int getNumFromArray(int dig[10]) {
    int res = 0;
    for(int i=0;i<9;i++) {
        res = res * 10 + dig[i];
    }
    return res;
}

void getArrayFromNum(int num, int dig[10]) {
    for (int i = 8; i >= 0; --i) {
        dig[i] = num % 10;
        num /= 10;
    }
}

int getIndex(int tile) {
    int res = 0;
    bool mem[10];
    memset(mem, false, sizeof mem);

    int t = 0, s[10];
    getArrayFromNum(tile, s);

    for (int i = 0; i < 9; i++) {
        int val = s[i];
        mem[val] = true;
        int cnt = 0;
        for (int j = 1; j < val; j++) {
            if (mem[j] == 0)
                cnt++;
        }
        res += cnt * factor[8 - i];
    }

    return res;
}

int getTile(int index) {

    bool mem[10];
    int s[10];
    memset(mem, false, sizeof mem);
    for (int i = 8; i >= 0; --i) {
        int gt = index / factor[i];
        index %= factor[i];
        int pos;
        for(int j = 1; j <= 9; ++j) {
            if(mem[j] == 0) {
                pos = j;
                break;
            }
        }

        while(gt--) {
            pos++;
            while(mem[pos]) {
                pos++;
            }
        }

        mem[pos] = true;
        s[8-i] = pos;
    }

    int tile = 0;
    for(int i=0;i<9;i++) {
        tile = tile * 10 + s[i];
    }

    return tile;
}

bool funcCheck(int x,int y) {
    if(x >= 0 && x < 3 && y >= 0 && y < 3) {
        return true;
    }
    return false;
};

// input a Near int[4]
int Near[4], dig[9];
void getNear(int index) {

    int tile = getTile(index);
    getArrayFromNum(tile, dig);

    int xPos = find(dig, dig + 9, 9) - dig;
    for (int d = 0; d < 4; d++) {
        int NewPos = xPos + DIR[d];
        int x1 = xPos / 3 + DIR[d] / 3, y1 = xPos % 3 + DIR[d] % 3;
        if (funcCheck(x1, y1)) {
            swap(dig[xPos], dig[NewPos]);
            int newTile = getNumFromArray(dig);
            int newIndex = getIndex(newTile);
            Near[d] = newIndex;
            swap(dig[xPos], dig[NewPos]);
        } else {
            Near[d] = -1;
        }
    }
}

// from, dir
struct Status {
    int last, dir;
    Status():last(-1), dir(-1){}
    Status(int a,int b) : last(a), dir(b) {}
}status[N];
int g[N];
bool vis[N];

struct AStarNode {
    int tile, index, g, h, f;
    AStarNode() {}
    AStarNode(int tile, int index, int g, int h): tile(tile), index(index), g(g), h(h){
        f = g+h;
    }
    bool operator < (const AStarNode& rhs) const{
        return f > rhs.f;
    }
};

int h(int tile) {
    getArrayFromNum(tile, dig);
    int dist = 0;
    for (int i = 0; i < 9; ++i) {
        int x1 = i / 3, y1 = i % 3;
        int x2 = (dig[i] - 1) / 3, y2 = (dig[i] - 1) % 3;
        dist += abs(x1 - x2) + abs(y1 - y2);
    }
    return dist;
}

string AStarSearch(int start, int end) {
    status[start] = Status(start, 0);
    int tile_start = getTile(start);
    g[start] = 0;
    priority_queue<AStarNode> pq;
    pq.push(AStarNode(tile_start, start, g[start], h(tile_start)));
    bool flagSuccess = false;
    vector<int> history;
    history.push_back(start);

    while (!pq.empty()) {
        AStarNode node = pq.top();
        pq.pop();
        if ( node.index == end ) {
            flagSuccess = true;
            break;
        }
        if (vis[node.index]) continue;
        vis[node.index] = true;

        getNear(node.index);
        for (int d = 0; d < 4; ++d) {
            int near_node = Near[d];
            if (near_node == -1 || vis[near_node]) continue;
            if (g[near_node] > node.g + 1) {
                g[near_node] = node.g + 1;
                int new_tile = getTile(near_node);
                AStarNode new_node = AStarNode(new_tile , near_node, g[near_node], h(new_tile));
                status[near_node] = Status(node.index, d);
                pq.push(new_node);
                history.push_back(new_node.index);
            }
        }
    }

    if (!flagSuccess) {
        return "unsolvable";
    }

    string move = "";
    int now = end;
    while(now != start) {
        char instruct = DIR_C[status[now].dir];
        move = instruct + move;
        now = status[now].last;
    }
#ifdef LOCAL
    cout << "search history size = " << history.size() << endl;
#endif
    for (int i = 0; i < history.size(); ++i) {
        status[history[i]] = Status();
        vis[history[i]] = false;
        g[history[i]] = inf;
    }
    return move;

}


void Init() {
    factor[0] = 1;
    for (int i = 1; i <= 9; i++)
        factor[i] = factor[i - 1] * i;

    memset(vis, false, sizeof vis);
    memset(g, inf, sizeof g);
}

void debug() {
    int input = getIndex(987654321);
    int tile = getTile(input);
    printf("%d : %d\n",input, tile);

    getNear(input);
    for(int d = 0; d < 4; d++) {
        int code = Near[d];
        printf("[%d], code = %d, ", DIR[d], code);
        if (code == -1) {
            puts("-1");
        } else {
            printf("%d\n", getTile(code));
        }
    }
}

int resPair(int tile) {
    getArrayFromNum(tile, dig);
    int resPair = 0;
    for (int i = 0; i < 9 ; i++) {
        for(int j = i + 1;j < 9; j++) {
            if (dig[i] == 9 || dig[j] == 9)
                continue;
            if(dig[i] > dig[j])
                resPair++;
        }
    }
    return resPair;
}

bool isChangeable(int start, int end){
    int startPairs = resPair(start);
    int endPairs = resPair(end);
    return startPairs % 2 == endPairs % 2;
};

int main() {
#ifdef LOCAL
    freopen("in", "r", stdin);
    clock_t start = clock();
#endif

    Init();
//    debug();
    while(true) {
        int inputTile = 0;
        bool finish = false;
        for(int i=0;i<9;i++) {
            string tile;
            if(!(cin >> tile)) {
                finish = true;
                break;
            }
            inputTile = inputTile * 10 + (tile == "x" ? 9 : tile[0] - '0');
        }
        if (finish)
            break;
        if (!isChangeable(inputTile, 123456789)) {
            puts("unsolvable");
            continue;
        }

        int start_index = getIndex(inputTile);
        int end_index = getIndex(123456789);

        string road = AStarSearch(start_index, end_index);

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