// g++ -std=gnu++20 -O2 -pipe -static -s delivery.cpp -o delivery
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
const i64 INF = (i64)4e15;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    if (!(cin >> n >> m))
        return 0;
    int sx, sy;
    cin >> sx >> sy;
    --sx;
    --sy;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i)
        cin >> grid[i];

    string s;
    cin >> s;

    // Список позиций для каждой буквы
    array<vector<int>, 26> pos; // храним как 1D-индекс x*m+y
    pos.fill({});
    for (int x = 0; x < n; ++x)
        for (int y = 0; y < m; ++y)
            pos[grid[x][y] - 'a'].push_back(x * m + y);

    const int NM = n * m;
    auto idx = [&](int x, int y)
    { return x * m + y; };

    // Буферы (1D)
    vector<i64> F(NM, INF);   // текущая стоимость быть в клетке
    vector<i64> TMP(NM, INF); // промежуточно (после столбцов)
    vector<i64> G(NM, INF);   // результат трансформа для шага
    vector<i64> line(max(n, m));

    F[idx(sx, sy)] = 0;

    auto dt1d = [&](i64 *a, int len, i64 *out)
    {
        // D[j] = min_i (a[i] + |j - i|)
        i64 best = INF;
        for (int j = 0; j < len; ++j)
        {
            best = min(best, a[j] - (i64)j);
            out[j] = (i64)j + best;
        }
        best = INF;
        for (int j = len - 1; j >= 0; --j)
        {
            best = min(best, a[j] + (i64)j);
            out[j] = min(out[j], -(i64)j + best);
        }
    };

    for (char ch : s)
    {
        // 1. трансформ по столбцам (x)
        // читаем столбец в line, прогоняем dt1d, пишем в TMP
        for (int y = 0; y < m; ++y)
        {
            for (int x = 0; x < n; ++x)
                line[x] = F[idx(x, y)];
            // dt1d на месте: вход line -> out тоже line (перепользуем память)
            static i64 outRow[300 + 5]; // n ≤ 300
            dt1d(line.data(), n, outRow);
            for (int x = 0; x < n; ++x)
                TMP[idx(x, y)] = outRow[x];
        }

        // 2. трансформ по строкам (y)
        for (int x = 0; x < n; ++x)
        {
            for (int y = 0; y < m; ++y)
                line[y] = TMP[idx(x, y)];
            static i64 outCol[300 + 5]; // m ≤ 300
            dt1d(line.data(), m, outCol);
            for (int y = 0; y < m; ++y)
                G[idx(x, y)] = outCol[y];
        }

        // 3. оставляем стоимости только на клетках нужного типа
        fill(F.begin(), F.end(), INF);
        int t = ch - 'a';
        for (int p : pos[t])
            F[p] = G[p];
    }

    // ответ — минимум среди клеток последнего типа
    i64 ans = INF;
    int t = s.back() - 'a';
    for (int p : pos[t])
        ans = min(ans, F[p]);
    cout << ans << "\n";
    return 0;
}
