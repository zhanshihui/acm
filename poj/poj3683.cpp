#include <iostream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <queue>
#define MAXN 2048
#define MAXM 10000000
using namespace std;
struct edge{
    int v, next;
};
class EDGE{
public:
    edge e[MAXM];
    int eid, p[MAXN], n;
    void init(){
        memset(p, -1, sizeof(p));
        eid = 0;
    }
    void insert(int x, int y){
        e[eid].v = y;
        e[eid].next = p[x];
        p[x] = eid++;
    }
}e1, e2;
int dfn[MAXN], low[MAXN], stack[MAXN], belong[MAXN];
int bcnt, dindex = 0, top = 0, n;
bool instack[MAXN];
void tarjan(int v){
    instack[v] = true;
    stack[++top] = v;
    dfn[v] = low[v] = ++dindex;
    for (int i = e1.p[v]; i != -1; i = e1.e[i].next)
        if (!dfn[e1.e[i].v]){
            tarjan(e1.e[i].v);
            low[v] = min(low[v], low[e1.e[i].v]);
        }
        else if (instack[e1.e[i].v]){
            low[v] = min(low[v], dfn[e1.e[i].v]);
        }
    int temp;
    if (dfn[v] == low[v]){
        ++bcnt;
        do{
            temp = stack[top--];
            instack[temp] = false;
            belong[temp] = bcnt;
        }while (temp != v);
    }
}
void solve(){
    memset(dfn, 0, sizeof(dfn));
    top = bcnt = dindex = 0;
    for (int i = 1; i <= n; ++i)
        if (!dfn[i]) tarjan(i);
}

bool intersect(int x1, int y1, int x2, int y2){
    return (x1 < y2) && (x2 < y1);
}
int x[MAXN], y[MAXN], a1, a2, b1, b2, d[MAXN];
bool init(){
    e1.init();
    if (scanf("%d", &n) == EOF) return false;
    e1.n = n;
    for (int i = 1; i <= n; ++i){
        scanf("%d:%d%d:%d%d", &a1, &a2, &b1, &b2, &d[i]);
        x[i] = a1 * 60 + a2; y[i] = b1 * 60 + b2;
    }
    for (int i = 1; i <= n; ++i)
        for (int j = i + 1; j <= n; ++j){
            if (intersect(x[i], x[i] + d[i], x[j], x[j] + d[j])){
                e1.insert(i, j + n); e1.insert(j, i + n);
            }
            if (intersect(x[i], x[i] + d[i], y[j] - d[j], y[j])){
                e1.insert(i, j); e1.insert(j + n, i + n);
            }
            if (intersect(y[i] - d[i], y[i], x[j], x[j] + d[j])){
                e1.insert(i + n, j + n); e1.insert(j, i);
            }
            if (intersect(y[i] - d[i], y[i], y[j] - d[j], y[j])){
                e1.insert(i + n, j); e1.insert(j + n, i);
            }
        }
    n *= 2;
    e1.n = n;
    return true;
}
int rd[MAXN], color[MAXN], opp[MAXN];
bool map[MAXN][MAXN];
void make_newmap(){
    memset(map, false, sizeof(map));
    e2.init();
    memset(rd, 0, sizeof(rd));
    for (int i = 1; i <= n; ++i)
        for (int j = e1.p[i]; j != -1; j = e1.e[j].next)
            map[belong[i]][belong[e1.e[j].v]] = true;
    e2.n = n = bcnt;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= n; ++j)
            if ((i != j) && map[i][j]){
                e2.insert(i, j); ++rd[j];
            }
}
void print_time(int min1, int min2){
    printf("%02d:%02d %02d:%02d\n", min1 / 60, min1 % 60, min2 / 60, min2 % 60);
}
int main(){
    while (init()){
        solve();
        bool flag = true;
        for (int i = 1; i <= n / 2; ++i)
            if (belong[i] == belong[i + n / 2]){
                flag = false; break;
            }
            else{
                opp[belong[i]] = belong[i + n / 2];
                opp[belong[i + n / 2]] = belong[i];
            }
        if (flag) printf("YES\n");
        else{
            printf("NO\n"); continue;
        }
        make_newmap();
        queue<int> q;
        for (int i = 1; i <= n; ++i)
            if (!rd[i]) q.push(i);
        memset(color, 0, sizeof(color));
        while (!q.empty()){
            int v = q.front();
            if (color[v] == 0){
                color[v] = 1; color[opp[v]] = -1;
            }
            q.pop();
            for (int i = e2.p[v]; i != -1; i = e2.e[i].next)
                if ((--rd[e2.e[i].v]) == 0) q.push(e2.e[i].v);
        }
        for (int i = 1; i <= e1.n / 2; ++i)
            if (color[belong[i]] == -1) print_time(x[i], x[i] + d[i]);
            else print_time(y[i] - d[i], y[i]);
    }
    return 0;
}
