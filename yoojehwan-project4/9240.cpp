#include <iostream>
#include <algorithm>
#include <vector>
#include <deque>
#include <tuple>
#include <cmath>

using namespace std;

typedef long long LL;
typedef tuple<long, long, double, double> Point;

int N;
vector<Point> points;

bool comp_y_x(Point &a, Point &b)
{
  if (get<1>(a) < get<1>(b))
    return true;
  else if (get<1>(a) == get<1>(b))
    return get<0>(a) < get<0>(b);
  else
    return false;
}

bool comp_angles(Point &a, Point &b)
{
  if (get<2>(a) < get<2>(b))
    return true;
  else if (get<2>(a) == get<2>(b))
    return get<3>(a) < get<3>(b);
  else
    return false;
}

double getDistance(const Point &p, const Point &s) {
  return sqrt(pow(get<0>(p) - get<0>(s), 2) + pow(get<1>(p) - get<1>(s), 2));
}

double getAngle(const Point &p, const Point &s) {
  double radian = atan2(get<1>(p) - get<1>(s), get<0>(p) - get<0>(s));
  return radian * 180 / M_PI;
}

void get_angle_distance()
{
  const Point &s = points[0];

  for_each(points.begin() + 1, points.end(), [&s](Point &p) {
    double radian = atan2(get<1>(p) - get<1>(s), get<0>(p) - get<0>(s));
    double angle = getAngle(p, s);
    double distance = getDistance(p, s);
    get<2>(p) = angle;
    get<3>(p) = distance;
  });
}

LL ccw(const Point &a, const Point &b, const Point &c)
{
  LL v1 = get<0>(a) * get<1>(b) + get<0>(b) * get<1>(c) + get<0>(c) * get<1>(a);
  LL v2 = get<0>(b) * get<1>(a) + get<0>(c) * get<1>(b) + get<0>(a) * get<1>(c);
  return v1 - v2;
}

double graham_scan()
{
  double maxval = 0;
  deque<Point> dq;

  dq.push_back(points[0]);
  dq.push_back(points[1]);

  for (int i = 2; i < points.size(); i++) {
    const Point next = points[i];
    while (dq.size() >= 2) {
      const Point second = dq.back();
      dq.pop_back();
      const Point first = dq.back();

      if (ccw(first, second, next) > 0) {
        dq.push_back(second);
        break;
      }
    }

    dq.push_back(next);
  }

  for (int i = 0; i < dq.size(); i++)
    for (int j = i + 1; j < dq.size(); j++)
      maxval = max(maxval, getDistance(dq[i], dq[j]));

  return maxval;
}

int main(int argc, char *argv[])
{
  int x, y;
  
  ios::sync_with_stdio(false);
  cout << fixed;
  cout.precision(8);

  cin >> N;

  for (int i = 0; i < N; i++) {
    cin >> x >> y;
    points.push_back(make_tuple(x, y, 0, 0));
  }

  sort(points.begin(), points.end(), comp_y_x);

  get_angle_distance();
  sort(points.begin(), points.end(), comp_angles);

  cout << graham_scan() << endl;
}