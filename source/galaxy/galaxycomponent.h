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
    virtual void componentIntensity(RasterPixel* rp, QVector3D& p, float ival );
    virtual float calculateIntensity(RasterPixel* rp,  QVector3D& p,
                                    GalaxyInstance* gi, const float weight);


    //virtuafloat getIntensityModifier();
    void setGalaxyParam(GalaxyParams* gp);
    float cosh(const float x);
    virtual float getHeightModulation(float height);
    virtual float getRadialIntensity(const float rad);
    float getRadius(const QVector3D& p, QVector3D& P, float& dott, GalaxyInstance* gi);
    QVector3D twirl( QVector3D& p,  const float twirl);
    float getPerlinCloudNoise(QVector3D& p, const float t, const int NN, const float ks, const float pers);
    float findDifference( float t1,  float t2);
    float calculateArmValue( float rad,  const QVector3D& P);
    float getArm( const float rad,  const QVector3D& p,  const float disp);
    float getTheta( const QVector3D& p );
    float getWinding( float rad);

    ComponentParams& getComponentParams();
    ComponentSpectrum* getSpectrum() const;
    void setSpectrum(ComponentSpectrum* spectrum);
};


inline float GalaxyComponent::cosh(const float x) {
    return ( exp (x) + exp (-x))/2.0;
}

inline float GalaxyComponent::getHeightModulation(float height) {
    float h = abs(height/m_componentParams.z0());
    if (h>2.0)
        return 0;

    float val = 0;
    val = 1.0f/cosh(h);
    height = val*val;
    return height;
}

inline float GalaxyComponent::getRadius(const QVector3D& p, QVector3D& P, float& dott, GalaxyInstance* gi)  {
    dott = QVector3D::dotProduct(p, gi->orientation());
    P = p - gi->orientation()*dott;
    return P.length()/ m_galaxyParams->axis().x();
}

inline float GalaxyComponent::getRadialIntensity(const float rad) {
    float r = exp(-rad/(m_componentParams.r0()*0.5f));
    return Util::clamp(r - 0.01f,0,1);
}




inline QVector3D GalaxyComponent::twirl( QVector3D& p,  const float twirl) {
    QQuaternion q =  QQuaternion::fromAxisAndAngle(m_currentGI->orientation(),twirl*180.0);
    //    return p;
    return q*p;
}


inline float GalaxyComponent::getPerlinCloudNoise(QVector3D& p, const float t, const int NN, const float ks, const float pers)
{
    QVector3D r = twirl(p, t);
    return m_galaxyParams->noise()->octave_noise_3d(NN, pers, ks * 0.1f, r.x(), r.y(), r.z());
    // return IQNoise.octave_noise_3d(NN, pers, ks * 0.1f, r.x, r.y, r.z);
}




inline float GalaxyComponent::findDifference( float t1,  float t2) {
    float v1 = abs(t1-t2);

    float v2 = abs(t1-t2-2*M_PI);
    float v3 = abs(t1-t2+2*M_PI);
    float v4 = abs(t1-t2-2*M_PI*2);
    float v5 = abs(t1-t2+2*M_PI*2);

    float v = min(v1,v2);
    v = min(v,v3);
    v = min(v,v4);
    v = min(v,v5);

    return v;

}

inline float GalaxyComponent::calculateArmValue( float rad,  const QVector3D& P) {


    float v1 = getArm(rad, P, m_galaxyParams->arm1());
    if (m_galaxyParams->noArms()==1)
        return v1;
    float v = max(v1, getArm(rad, P, m_galaxyParams->arm2()));
    if (m_galaxyParams->noArms()==2)
        return v;

    v = max(v, getArm(rad, P, m_galaxyParams->arm3()));
    if (m_galaxyParams->noArms()==3)
        return v;

    v = max(v, getArm(rad, P, m_galaxyParams->arm4()));

    return v;
}


inline float GalaxyComponent::getArm( const float rad,  const QVector3D& p,  const float disp) {
    float m_workWinding = getWinding(rad);
    float m_workTheta = -getTheta(p);

    float v = abs(findDifference(m_workWinding,m_workTheta + disp))/M_PI;
    return pow(1.0-v,m_componentParams.arm()*15);
}



inline float GalaxyComponent::getTheta( const QVector3D& p ) {
    QVector3D quatRot = m_currentGI->rotmat()*p;
    return atan2(quatRot.x(), quatRot.z()) + m_componentParams.delta();
}


inline float GalaxyComponent::getWinding( float rad) {
    float r = rad + 0.05;

    float t = atan(exp(-0.25/(0.5*(r)))/m_galaxyParams->windingB())*2*m_galaxyParams->windingN();
    float scale = 1.0;
    float t2 = 0.0;
    t = t*scale + t2*(1-scale);
    return t;

}

