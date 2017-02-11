#ifndef IQNOISE_H
#define IQNOISE_H

#include <QVector3D>
#include "source/noise/noise.h"

class IQnoise : public Noise
{
    static inline double frac(const double a);
    static inline QVector3D frac(const QVector3D& a);
    static inline double iqhashStatic(const double n);
    static inline double lerp(const double a, const double b,const double w);
    static inline QVector3D floor(const QVector3D& a);

    double noise(const QVector3D x);


public:
    IQnoise(int o,double f,double p,int s) : Noise( o,f,p,s)  { }


    double raw_2d(const double& x, const double& y) override {
        return noise(QVector3D(x,y,0));
    }

    double raw_3d(const double &x, const double &y, const double& z) override {
        return noise(QVector3D(x,y,z));
    }



};

#endif // IQNOISE_H
