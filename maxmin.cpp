#include "recognizealgs.h"
#include <algorithm>
#include <functional>
#include <cmath>

#define RETURN_IF(fi, what) if(fi)return what

namespace maxmin_ns{

QVector<Point>::iterator farestPoint(Point& point, QVector<Point>& points)
{
    QVector<Point>::iterator ret = points.begin();
    double farestDistance = distance(point, *ret);

    for(QVector<Point>::iterator i = points.begin() + 1; i != points.end(); i++){
        double td = distance(point, *i);
        if((td > farestDistance) && (point != *i)){
            farestDistance = td;
            ret = i;
        }
    }
    return ret;
}

QVector<Point>::iterator nearestPoint(Point& point, QVector<Point>& points)
{
    QVector<Point>::iterator ret = points.begin();
    double farestDistance = distance(point, *ret);

    for(QVector<Point>::iterator i = points.begin() + 1; i != points.end(); i++){
        double td = distance(point, *i);
        if((td < farestDistance) && (point != *i)){
            farestDistance = td;
            ret = i;
        }
    }
    return ret;
}

double averageDistance(QVector<Point>& v)
{
    double ret = 0;
    double td = 0;

    for(QVector<Point>::iterator i = v.begin(); i + 1 != v.end(); i++){
        td += distance(*i, *(i+1));
    }

    ret = td / (double)v.size();

    return ret;
}

QVector<Point>::iterator center(QVector<Point>& points, QVector<Point>& center)
{
    QVector<Point>::iterator ret = points.begin();
    double maxdistance = distance(*ret, *nearestPoint(*ret, center));

    for(QVector<Point>::iterator i = points.begin() + 1; i != points.end(); i++){
        double td = distance(*i, *nearestPoint(*i, center));
        if((td > maxdistance) && (td > averageDistance(center)) && (*ret != *i)){
            maxdistance = td;
            ret = i;
        }
    }

    return ret;
}

}

QVector<Cluster> maxmin(QVector<Cluster> &points_, double param)
{
    using namespace maxmin_ns;

    QVector<Point> points;
    QVector<Point> centers;
    QVector<Cluster> clusters;

    foreach(Cluster c, points_)
    {
        points.push_front(c.center);
    }

    QVector<Point>::iterator firstPoint = points.begin();
    centers.push_front(*firstPoint);
    clusters.push_front(*firstPoint);
    points.erase(firstPoint);

    RETURN_IF(!points.size(), clusters);

    QVector<Point>::iterator secondPoint = farestPoint(*firstPoint, points);
    centers.push_front(*secondPoint);
    clusters.push_front(*secondPoint);
    points.erase(secondPoint);

    RETURN_IF(!points.size(), clusters);

    double avd1 = averageDistance(centers);

    QVector<Point>::iterator thirdPoint = center(points, centers);
    centers.push_front(*thirdPoint);
    clusters.push_front(*thirdPoint);
    points.erase(thirdPoint);

    RETURN_IF(!points.size(), clusters);

    double avd2 = averageDistance(centers);

    while(avd2 / avd1 < param){
        QVector<Point>::iterator point = center(points, centers);
        centers.push_front(*point);
        clusters.push_front(*point);
        points.erase(point);
        avd1 = avd2;
        avd2 = averageDistance(centers);
    }

    return clusters;
}
