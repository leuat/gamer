#ifndef PERLIN_H_

#define PERLIN_H_

#include <stdlib.h>
#include "noise.h"


#define SAMPLE_SIZE 1024


class Perlin : public Noise
{
    Q_OBJECT
public:

    //  Perlin(int octaves,double freq,double amp,int seed);
    double vec2[2];
    double vec3[3];

    Perlin(int o,double f,double p,int s) : Noise( o,f,p,s)  {
        srand(s);
        init();
    }


    /*  double Get(const double x,const double y)
  {
    vec2[0] = x;
    vec2[1] = y;
    return perlin_noise_2D(vec2);
  };

  double Get(const double x,const double y, const double z)
  {
    vec3[0] = x;
    vec3[1] = y;
    vec3[2] = z;
    return perlin_noise_3D(vec3);
  };
  */
    double raw_2d(const double& x, const double& y) {
        vec2[0] = x;
        vec2[1] = y;
        return 2*noise2(vec2);
    }

    double raw_3d(const double &x, const double &y, const double& z) {
        vec3[0] = x;
        vec3[1] = y;
        vec3[2] = z;
        return 2*noise3(vec3);
    }



private:
    void init_perlin(int n,double p);
    double perlin_noise_2D(double vec[2]);
    double perlin_noise_3D(double vec[3]);

    double noise1(double arg);
    double noise2(double vec[2]);
    double noise3(double vec[3]);
    void normalize2(double v[2]);
    void normalize3(double v[3]);
    void init(void);

    int   mOctaves;
    double mFrequency;
    double mAmplitude;
    int   mSeed;

    int p[SAMPLE_SIZE + SAMPLE_SIZE + 2];
    double g3[SAMPLE_SIZE + SAMPLE_SIZE + 2][3];
    double g2[SAMPLE_SIZE + SAMPLE_SIZE + 2][2];
    double g1[SAMPLE_SIZE + SAMPLE_SIZE + 2];
    bool  mStart;

};



#endif

