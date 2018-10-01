#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

struct target_t {
    int x, y, w, h;
    target_t(const int &_x, const int &_y, const int &_w, const int &_h) : x(_x), y(_y), w(_w), h(_h) {}
    bool operator != (const target_t &rhs) const {
        return x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h;
    }
    bool operator < (const target_t &rhs) const {
        return y < rhs.y;
    }
};

int main()
{
    int i, j, k, n, x, y, w, h, dist, max_dist = 0, c1 = -1, c2 = -1;
    double p1, p2;
    cin >> n;
    vector<target_t> targets;
    for(i = 1; i <= n; i++)
    {
        cin >> x >> y >> w >> h;
        if(fabs(h*1.0/w - 5.0) <= 0.5)
        {
            targets.emplace_back(x, y, w, h);
        }
    }
    sort(targets.begin(), targets.end());
    for(i = 0; i < targets.size(); i++)
    {
        for(j = i+1; j < targets.size(); j++)
            if(targets[i].y != targets[j].y)
                break;
        p1 = targets[k].h / 10.0;
        for(k = i+1; k < j; k++)
        {
            p2 = targets[k].h / 10.0;
            dist = abs(targets[k].x - targets[i].x);
            if(targets[i] != targets[k] && fabs(p2 - p1) <= 0.5 && fabs(dist / ((p1 + p2) / 2) - 15.0) <= 0.5 && dist > max_dist)
            {
                max_dist = dist;
                c1 = i;
                c2 = k;
            }
        }
    }
    if(c1 >= 0 && c2 >= 0)
    {
        cout << "[(" << targets[c1].x << ',' << targets[c1].y << ',' << targets[c1].w << ',' << targets[c1].h << "),";
        cout <<  "(" << targets[c2].x << ',' << targets[c2].y << ',' << targets[c2].w << ',' << targets[c2].h << ")]" << endl;
    }
    else
    {
        cout << "Appropriate target not found" << endl;
    }
    return 0;
}
