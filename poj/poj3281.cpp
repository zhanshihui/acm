#include <iostream>
#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>
using namespace std;
typedef struct
{
    int v, next, val;
}edge;
const int MAXN = 1024;
const int MAXM = 1000000;
edge e[MAXM];
int p[MAXN], eid;
inline void sapinit()
{
    memset(p, -1, sizeof(p));
    eid = 0;
}
inline void insert1(int from, int to, int val)
{
    e[eid].v = to;
    e[eid].val = val;
    e[eid].next = p[from];
    p[from] = eid++;
    swap(from, to);
    e[eid].v = to;
    e[eid].val = 0;
    e[eid].next = p[from];
    p[from] = eid++;
}
inline void insert2(int from, int to, int val)
{
    e[eid].v = to;
    e[eid].val = val;
    e[eid].next = p[from];
    p[from] = eid++;
    swap(from, to);
    e[eid].v = to;
    e[eid].val = val;
    e[eid].next = p[from];
    p[from] = eid++;
}
int n, m;
int h[MAXN], gap[MAXN];
int source, sink;
inline int dfs(int pos, int cost)
{
    if (pos == sink)
    {
        return cost;
    }
    int j, minh = n - 1, lv = cost, d;
    for (j = p[pos]; j != -1; j = e[j].next)
    {
        int v = e[j].v, val = e[j].val;
        if (val > 0)
        {
            if (h[v] + 1 == h[pos])
            {
                if (lv < e[j].val) d = lv;
                else d = e[j].val;
                d = dfs(v, d);
                e[j].val -= d;
                e[j ^ 1].val += d;
                lv -= d;
                if (h[source] >= n) return cost - lv;
                if (lv == 0) break;
            }
            if (h[v] < minh) minh = h[v];
        }
    }
    if (lv == cost)
    {
        --gap[h[pos]];
        if (gap[h[pos]] == 0) h[source] = n;
        h[pos] = minh + 1;
        ++gap[h[pos]];
    }
    return cost - lv;
}
int sap(int st, int ed)
{
    source = st;
    sink = ed;
    int ret = 0;
    memset(gap, 0, sizeof(gap));
    memset(h, 0, sizeof(h));
    gap[st] = n;
    while (h[st] < n)
    {
        ret += dfs(st, INT_MAX);
    }
    return ret;
}

int c, f, d, s1, s2, x, s, t;
bool init()
{
    sapinit();
    if (scanf("%d%d%d", &c, &f, &d) == EOF) return false;
    for (int i = 1; i <= c; ++i)
    {
        scanf("%d%d", &s1, &s2);
        while (s1--)
        {
            scanf("%d", &x);
            insert1(x + 2 * c, i, 1);
        }
        while (s2--)
        {
            scanf("%d", &x);
            insert1(i + c, 2 * c + f + x, 1);
        }
    }
    for (int i = 1; i <= c; ++i)
        insert1(i, i + c, 1);
    s = c * 2 + f + d + 1;
    t = s + 1;
    for (int i = 1; i <= f; ++i)
        insert1(s, i + 2 * c, 1);
    for (int i = 1; i <= d; ++i)
        insert1(2 * c + f + i, t, 1);
    n = t;
    return true;
}

int main()
{
    while (init())
    {
        printf("%d\n", sap(s, t));
    }
    return 0;
}
