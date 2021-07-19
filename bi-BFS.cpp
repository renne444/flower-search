// 双向广搜 RESULT:
// POJ-1077 Accepted 32ms
// HDU-1043 Time Limited Exceed
#include <cstdio>
#include <string>
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <map>
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

bool funcCheck(int x,int y){
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


void Init() {
    factor[0] = 1;
    for (int i = 1; i <= 9; i++)
        factor[i] = factor[i - 1] * i;
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

// from, dir
struct Status {
    int last, dir;
    Status():last(-1), dir(-1){}
    Status(int a,int b) : last(a), dir(b) {}
}in_order[N], rev_order[N];

string bi_direct_BFS(int start, int end) {
    in_order[start] = Status(start, 0);
    rev_order[end] = Status(end, 0);
    queue<int> in_que, rev_que;
    in_que.push(start);
    rev_que.push(end);
    int mid_node = -1;
    vector<int> in_history, rev_history;
    string ans = "unsolvable";

    while(!in_que.empty() || !rev_que.empty()) {
        if (!in_que.empty()) {
            int in_node = in_que.front();
            in_que.pop();
            getNear(in_node);
            for(int d = 0; d < 4; ++d) {
                if (Near[d] != -1 && in_order[Near[d]].last == -1) {
                    in_order[Near[d]] = Status(in_node, d);
                    in_que.push(Near[d]);
                    in_history.push_back(Near[d]);
                    if (rev_order[Near[d]].last != -1) {
                        mid_node = Near[d];
                    }
                }
            }
        }

        if (mid_node == -1 && !rev_que.empty()) {
            int rev_node = rev_que.front();
            rev_que.pop();
            getNear(rev_node);
            for (int d = 0; d < 4; ++d) {
                if (Near[d] != -1 && rev_order[Near[d]].last == -1) {
                    rev_order[Near[d]] = Status(rev_node, d);
                    rev_que.push(Near[d]);
                    rev_history.push_back(Near[d]);
                    if (in_order[Near[d]].last != -1) {
                        mid_node = Near[d];
                    }
                }
            }
        }

        if (mid_node != -1) {
            string in_ans = "";
            int now_node = mid_node;
            while (now_node != start) {
                char instruct = DIR_C[in_order[now_node].dir];
                in_ans = instruct + in_ans;
                now_node = in_order[now_node].last;
            }

            string rev_ans = "";
            now_node = mid_node;
            while(now_node != end) {
                char instruct = DIR_C[rev_order[now_node].dir ^ 1];
                rev_ans += instruct;
                now_node = rev_order[now_node].last;
            }

            ans = in_ans + rev_ans;
            break;
        }
    }
    for (int i = 0; i < in_history.size(); ++i) {
        int in_node = in_history[i];
        in_order[in_node] = Status();
    }
    for (int i = 0; i < rev_history.size(); ++i) {
        int rev_node = rev_history[i];
        rev_order[rev_node] = Status();
    }

    return ans;
}

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

//        string road = bfs(start_index, end_index);
        string road = bi_direct_BFS(start_index, end_index);

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