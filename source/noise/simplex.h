#pragma once

#include "source/noise/noise.h"
#include "source/noise/simplexnoise.h"
/*
 * Simplex noise class. From Ken Perlin.
 *
 *
*/
class Simplex : public Noise {
    Q_OBJECT
public:

    Simplex(int o,double f,double p,int s) : Noise( o,f,p,s)  { }


    double raw_2d(const double& x, const double& y);
    double raw_3d(const double& x, const double& y, const double& z);


};
