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

struct P {
    P(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    friend P operator-(const P& p1, const P& p2) {
        return P(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
    }

    double x, y, z;
};

struct Sphere {
    Sphere(const P& center = P(), double r = 0) : center(center), r(r) {}

    P center;
    double r;
};

struct Light {
    Light(const P& p = P(), double bright = 0) : p(p), bright(bright) {}

    P p;
    double bright;
};

inline double dot(const P& p1, const P& p2)
{
    return p1.x * p2.x + p1.y * p2.y + p1.z * p2.z;
}

inline double dist2(const P& p1, const P& p2)
{
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
}

bool isCrossed(const Sphere& sphere, const P& p1, const P& p2)
{
    bool p1IsInSphere = dist2(sphere.center, p1) <= (sphere.r * sphere.r);
    bool p2IsInSphere = dist2(sphere.center, p2) <= (sphere.r * sphere.r);

    if (p1IsInSphere && p2IsInSphere)
        return false;

    if ((p1IsInSphere && !p2IsInSphere) || (!p1IsInSphere && p2IsInSphere))
        return true;

    double ph = dot(sphere.center - p1, p2 - p1) / sqrt(dist2(p1, p2));
    if (ph < 0)
        return false;

    double hc2 = dist2(sphere.center, p1) - ph * ph;
    if (hc2 > sphere.r * sphere.r)
        return false;

    if (dist2(p1, p2) < ph * ph)
        return false;
    else
        return true;
}

double solve(const vector<Sphere>& spheres, const vector<Light>& lights, const P& targetPoint, int R)
{
    double result = 0;
    for (int bits = 0; bits < (1 << lights.size()); ++bits) {
        double brightness = 0;
        for (int i = 0; i < lights.size(); ++i) {
            if ((bits & (1 << i)) != 0)
                brightness += lights[i].bright / dist2(lights[i].p, targetPoint);
        }

        if (brightness < result)
            continue;

        vector<bool> removedSpheres(spheres.size(), false);
        int restSpheres = R;
        for (int i = 0; i < lights.size(); ++i) {
            if ((bits & (1 << i)) == 0)
                continue;
            for (int j = 0; j < spheres.size(); ++j) {
                if (!removedSpheres[j] && isCrossed(spheres[j], targetPoint, lights[i].p)) {
                    restSpheres -= 1;
                    removedSpheres[j] = true;

                    if (restSpheres < 0)
                        goto NEXT;
                }
            }
        }

        result = max(result, brightness);
    NEXT:
        ;
    }

    return result;
}

int main(void)
{
    for (int N, M, R; cin >> N >> M >> R, (N || M || R); ) {
        vector<Sphere> spheres;
        for (int i = 0; i < N; ++i) {
            int x, y, z, r; cin >> x >> y >> z >> r;
            spheres.push_back(Sphere(P(x, y, z), r));
        }

        vector<Light> lights;
        for (int i = 0; i < M; ++i) {
            int x, y, z, b; cin >> x >> y >> z >> b;
            lights.push_back(Light(P(x, y, z), b));
        }

        int x, y, z; cin >> x >> y >> z;
        P targetPoint(x, y, z);

        char buf[80];
        sprintf(buf, "%.5f", solve(spheres, lights, targetPoint, R));
        cout << buf << endl;
    }
    return 0;
}
