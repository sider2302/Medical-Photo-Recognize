#ifndef RECOGNIZEALGS_H
#define RECOGNIZEALGS_H

#include <QPointF>
#include <QColor>
#include <QVector>
#include <QDataStream>
#include <cmath>

struct Lab
{
    double l;
    double a;
    double b;
};

struct Point: public QPointF, public QColor
{
    Point(){

    }

    Point(const QPointF& p):QPointF(p){

    }

    Point(const QColor& c):QColor(c){

    }

    explicit Point(const QColor& c, const QPointF& p): QPointF(p), QColor(c){

    }

    Point(const Point& p): QPointF(p), QColor(p){

    }

    operator QPointF(){
        return QPointF(x(), y());
    }

    operator QColor(){
        return QColor(rgba());
    }

    inline friend QDataStream& operator << (QDataStream& stream, const Point& c){
        stream << c;
        return stream;
    }

    inline friend QDataStream& operator >> (QDataStream& stream, Point& c){
        stream >> c;
        return stream;
    }

    inline friend bool operator != (const Point& p1, const Point& p2){
        return p1.x() != p2.x() && p1.y() != p2.y() &&
                p1.rgba() != p2.rgba();
    }

    inline friend bool operator == (const Point& p1, const Point& p2){
        return !(p1 != p2);
    }

    Lab lab() const
    {
        double xyz_r = (double)red() / 255.0;
        double xyz_g = (double)green() / 255.0;
        double xyz_b = (double)blue() / 255.0;

        double xyz_x;
        double xyz_y;
        double xyz_z;

        double lab_l, lab_a, lab_b;

        if ( xyz_r > 0.04045 ) xyz_r = pow(( ( xyz_r + 0.055 ) / 1.055 ), 2.4);
        else xyz_r = xyz_r / 12.92;

        if ( xyz_g > 0.04045 ) xyz_g = pow(( ( xyz_g + 0.055 ) / 1.055 ), 2.4);
        else xyz_g = xyz_g / 12.92;

        if ( xyz_b > 0.04045 ) xyz_b = pow(( ( xyz_b + 0.055 ) / 1.055 ), 2.4);
        else xyz_b = xyz_b / 12.92;

        xyz_r = xyz_r * 100;
        xyz_g = xyz_g * 100;
        xyz_b = xyz_b * 100;

        xyz_x = xyz_r * 0.4124 + xyz_g * 0.3576 + xyz_b * 0.1805;
        xyz_y = xyz_r * 0.2126 + xyz_g * 0.7152 + xyz_b * 0.0722;
        xyz_z = xyz_r * 0.0193 + xyz_g * 0.1192 + xyz_b * 0.9505;

        //перевод XYZ в LAB
        xyz_x = xyz_x / 95.047;
        xyz_y = xyz_y / 100.000;
        xyz_z = xyz_z / 108.883;

        if ( xyz_x > 0.008856 ) xyz_x = pow(xyz_x, ( 1.0/3.0 ));
        else xyz_x = ( 7.787 * xyz_x ) + ( 16.0 / 116.0 );

        if ( xyz_y > 0.008856 ) xyz_y = pow(xyz_y, ( 1.0/3.0 ));
        else xyz_y = ( 7.787 * xyz_y ) + ( 16.0 / 116.0 );

        if ( xyz_z > 0.008856 ) xyz_z = pow(xyz_z, ( 1.0/3.0 ));
        else xyz_z = ( 7.787 * xyz_z ) + ( 16.0 / 116.0 );

        lab_l = ( 116 * xyz_y ) - 16;
        lab_a = 500 * ( xyz_x - xyz_y);
        lab_b = 200 * ( xyz_y - xyz_z );

        Lab lab = {lab_l, lab_a, lab_b};

        return lab;
    }

    friend double distance(const Point& p1, const Point& p2){
        Lab lab1 = p1.lab();
        Lab lab2 = p2.lab();

        return sqrt(pow(lab2.l - lab1.l, 2) + pow(lab2.a - lab1.a, 2) + pow(lab2.b - lab1.b, 2));
    }

    virtual ~Point(){

    }
};

typedef Point Center;
typedef QVector<Point> PointsCluster;

struct Cluster
{
    Cluster()
    {
        center = Center(QPointF(-20, -20));
    }
    Cluster(const Center& c): center(c){}
    Center center;
    PointsCluster pointsCluster;

    inline friend bool operator < (Cluster c1, Cluster c2)
    {
        return c1.center.x() < c2.center.x() && c1.center.y() < c2.center.y();
    }

    inline friend bool operator == (Cluster c1, Cluster c2)
    {
        return c1.center.x() == c2.center.x() && c1.center.y() == c2.center.y();
    }

    inline friend QDataStream& operator << (QDataStream& stream, const Cluster& c){
        stream << c.center;
        stream << c.pointsCluster;
        return stream;
    }

    inline friend QDataStream& operator >> (QDataStream& stream, Cluster& c){
        stream >> c.center;
        stream >> c.pointsCluster;
        return stream;
    }

};

QVector<Cluster> maxmin(QVector<Cluster> &, double);

#endif // RECOGNIZEALGS_H
