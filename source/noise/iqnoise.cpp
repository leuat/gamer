#include "iqnoise.h"
#include <math.h>


double IQnoise::frac(const double a)
{

    return a - std::floor(a);
}

QVector3D IQnoise::frac(const QVector3D& a)
{
    return QVector3D(a.x() - std::floor(a.x()), a.y() - std::floor(a.y()), a.z() - std::floor(a.z()));
}

QVector3D IQnoise::floor(const QVector3D& a)
{
    return QVector3D(std::floor(a.x()),std::floor(a.y()), std::floor(a.z()));
}

double IQnoise::iqhashStatic(const double n)
{
    return (double)frac(sin(n) * 753.5453123);
}


double IQnoise::lerp(const double a, const double b,const double w)
{
    return a + w * (b - a);
}



double IQnoise::noise(const QVector3D x)
{
    // The noise function returns a value in the range -1.0f -> 1.0f
    QVector3D p = floor(x);
    QVector3D f = frac(x);

    f.setX(f.x() * f.x() * (3.0f - 2.0f * f.x()));
    f.setY(f.y() * f.y() * (3.0f - 2.0f * f.y()));
    f.setZ(f.z() * f.z() * (3.0f - 2.0f * f.z()));


    double n = (p.x() + p.y() * 157.0f + 113.0f * p.z());

    return lerp(lerp(lerp(iqhashStatic(n + 0.0f), iqhashStatic(n + 1.0f), f.x()),
        lerp(iqhashStatic(n + 157.0f), iqhashStatic(n + 158.0f), f.x()), f.y()),
        lerp(lerp(iqhashStatic(n + 113.0f), iqhashStatic(n + 114.0f), f.x()),
            lerp(iqhashStatic(n + 270.0f), iqhashStatic(n + 271.0f), f.x()), f.y()), f.z());


}
