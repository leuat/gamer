#pragma once

#include <QString>
#include <QVector3D>
#include <QQuaternion>
#include <math.h>

#include "source/galaxy/galaxy.h"
#include "source/galaxy/componentparams.h"
#include "source/galaxy/galaxyparams.h"
#include "source/galaxy/spectrum.h"
#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/rasterizer.h"
#include "source/util/util.h"


class GalaxyComponent {

protected:
    GalaxyParams* m_galaxyParams = nullptr;

    ComponentParams m_componentParams;
    ComponentSpectrum* m_spectrum = nullptr;

    GalaxyInstance* m_currentGI = nullptr;
    QVector3D m_dAdd = QVector3D(0,0,0);

public:

    GalaxyComponent();
    void Initialize(ComponentParams* cp, GalaxyParams* gp);
    virtual void componentIntensity(RasterPixel* rp, QVector3D& p, double ival );
    virtual double calculateIntensity(RasterPixel* rp,  QVector3D& p,
                                    GalaxyInstance* gi, const double weight);


    //virtuadouble getIntensityModifier();
    void setGalaxyParam(GalaxyParams* gp);
    double cosh(const double x);
    virtual double getHeightModulation(double height);
    virtual double getRadialIntensity(const double rad);
    double getRadius(const QVector3D& p, QVector3D& P, double& dott, GalaxyInstance* gi);
    QVector3D twirl( QVector3D& p,  const double twirl);
    double getPerlinCloudNoise(QVector3D& p, const double t, const int NN, const double ks, const double pers);
    double findDifference( double t1,  double t2);
    double calculateArmValue( double rad,  const QVector3D& P);
    double getArm( const double rad,  const QVector3D& p,  const double disp);
    double getTheta( const QVector3D& p );
    double getWinding( double rad);

    ComponentParams& getComponentParams();
    ComponentSpectrum* getSpectrum() const;
    void setSpectrum(ComponentSpectrum* spectrum);
};


inline double GalaxyComponent::cosh(const double x) {
    return ( exp (x) + exp (-x))/2.0;
}

inline double GalaxyComponent::getHeightModulation(double height) {
    double h = abs(height/m_componentParams.z0());
    if (h>2.0)
        return 0;

    double val = 0;
    val = 1.0f/cosh(h);
    height = val*val;
    return height;
}

inline double GalaxyComponent::getRadius(const QVector3D& p, QVector3D& P, double& dott, GalaxyInstance* gi)  {
    dott = QVector3D::dotProduct(p, gi->orientation());
    P = p - gi->orientation()*dott;
    return P.length()/ m_galaxyParams->axis().x();
}

inline double GalaxyComponent::getRadialIntensity(const double rad) {
    double r = exp(-rad/(m_componentParams.r0()*0.5f));
    return Util::clamp(r - 0.01f,0,1);
}




inline QVector3D GalaxyComponent::twirl( QVector3D& p,  const double twirl) {
    QQuaternion q =  QQuaternion::fromAxisAndAngle(m_currentGI->orientation(),twirl*180.0);
    //    return p;
    return q*p;
}


inline double GalaxyComponent::getPerlinCloudNoise(QVector3D& p, const double t, const int NN, const double ks, const double pers)
{
    QVector3D r = twirl(p, t);
    return m_galaxyParams->noise()->octave_noise_3d(NN, pers, ks * 0.1f, r.x(), r.y(), r.z());
    // return IQNoise.octave_noise_3d(NN, pers, ks * 0.1f, r.x, r.y, r.z);
}




inline double GalaxyComponent::findDifference( double t1,  double t2) {
    double v1 = abs(t1-t2);

    double v2 = abs(t1-t2-2*M_PI);
    double v3 = abs(t1-t2+2*M_PI);
    double v4 = abs(t1-t2-2*M_PI*2);
    double v5 = abs(t1-t2+2*M_PI*2);

    double v = fmin(v1,v2);
    v = fmin(v,v3);
    v = fmin(v,v4);
    v = fmin(v,v5);

    return v;

}

inline double GalaxyComponent::calculateArmValue( double rad,  const QVector3D& P) {


    double v1 = getArm(rad, P, m_galaxyParams->arm1());
    if (m_galaxyParams->noArms()==1)
        return v1;
    double v = fmax(v1, getArm(rad, P, m_galaxyParams->arm2()));
    if (m_galaxyParams->noArms()==2)
        return v;

    v = fmax(v, getArm(rad, P, m_galaxyParams->arm3()));
    if (m_galaxyParams->noArms()==3)
        return v;

    v = fmax(v, getArm(rad, P, m_galaxyParams->arm4()));

    return v;
}


inline double GalaxyComponent::getArm( const double rad,  const QVector3D& p,  const double disp) {
    double m_workWinding = getWinding(rad);
    double m_workTheta = -getTheta(p);

    double v = abs(findDifference(m_workWinding,m_workTheta + disp))/M_PI;
    return pow(1.0-v,m_componentParams.arm()*15);
}



inline double GalaxyComponent::getTheta( const QVector3D& p ) {
    QVector3D quatRot = m_currentGI->rotmat()*p;
    return atan2(quatRot.x(), quatRot.z()) + m_componentParams.delta();
}


inline double GalaxyComponent::getWinding( double rad) {
    double r = rad + 0.05;

    double t = atan(exp(-0.25/(0.5*(r)))/m_galaxyParams->windingB())*2*m_galaxyParams->windingN();
    double scale = 1.0;
    double t2 = 0.0;
    t = t*scale + t2*(1-scale);
    return t;

}

