#include "source/galaxy/galaxycomponent.h"
#include "source/util/util.h"
#include "source/noise/simplexnoise.h"

ComponentParams GalaxyComponent::getComponentParams() const
{
    return m_componentParams;
}

GalaxyComponent::GalaxyComponent() {

}


void GalaxyComponent::Initialize(ComponentParams* cp, GalaxyParams* gp) {
    m_componentParams = *cp;
    m_galaxyParams = gp;
    m_count = 0;
    m_average = 0;
    //m_spectrum = Spectra.FindSpectrum(cp.spectrum);
    }


void GalaxyComponent::componentIntensity(RasterPixel* rp, QVector3D& p, float ival )  {
        qDebug() << "GALAXYCOMPONENT: COMPONENTINTENSITY SHOULD NEVER BE CALLED";
        rp->setI(QVector3D(0,0,0));
    }
    // returns the intensity at a given position

void GalaxyComponent::setGalaxyParam(GalaxyParams* gp) {
        m_galaxyParams = gp;
    }

float GalaxyComponent::cosh(const float x) {
    return ( exp (x) + exp (-x))/2.0;
}

float GalaxyComponent::getHeightModulation(float height) {
    float h = abs(height/m_componentParams.z0());
    float val = 0;
    if (abs(h)>2.0) {
        height = 0;
        return 0;
    }

    val = 1.0f/cosh(h);
    height = val*val;
    return height;
}


void GalaxyComponent::calculateIntensity(RasterPixel* rp, QVector3D& p,
                                GalaxyInstance* gi, const float weight) {
        QVector3D P;
        m_currentGI = gi;
        float z = 1;
        m_currentRadius = getRadius(p, P, z, gi);
        z = getHeightModulation(z);
        float armVal = 1;
         if (z>0.01)
         {

            float intensity = getRadialIntensity(m_currentRadius);
            if (intensity>0.1) intensity = 0.1;
            if (intensity >0.001) {

                float scale = 1;
                if (m_componentParams.className() == "Dust")
                    scale = Util::smoothstep(0, 1.0f*m_galaxyParams->bulgeDust(), m_currentRadius);
                if (m_componentParams.arm()!=0) {
                    armVal = calculateArmValue(m_currentRadius, P, m_componentParams.arm());
                    m_winding = getWinding(m_currentRadius)*m_componentParams.winding();///(rad+1.0);

                }


                // equation 5 from the paper
                float val = (m_componentParams.strength())*scale*armVal*z*intensity*gi->intensityScale();
                if (val * weight > 0.0005)
                {
//                        noise
                    componentIntensity(rp, p, val * weight);
                }
            }
        }
    }

float GalaxyComponent::getRadialIntensity(const float rad) {
    float r = exp(-rad/(m_componentParams.r0()*0.5f));
    return Util::clamp(r - 0.01f,0,1);
    }


float GalaxyComponent::getRadius(QVector3D p, QVector3D& P, float& dott, GalaxyInstance* gi)  {
    dott = QVector3D::dotProduct(p, gi->orientation());
        P = p - gi->orientation()*dott;
        return P.length()/ m_galaxyParams->axis().x();
    }


QVector3D GalaxyComponent::twirl( QVector3D& p,  const float twirl) {
    QQuaternion q =  QQuaternion::fromAxisAndAngle(m_currentGI->orientation(),twirl*180.0);
    return q*p;
}


float GalaxyComponent::getPerlinCloudNoise(QVector3D& p, const float rad, const float t, const int NN, const float ks, const float pers)
    {
        QVector3D r = twirl(p, t);
        return octave_noise_3d(NN, pers, ks * 0.1f, r.x(), r.y(), r.z());
//            return IQNoise.octave_noise_3d(NN, pers, ks * 0.1f, r.x, r.y, r.z);
    }


float GalaxyComponent::findDifference( float t1,  float t2) {
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
float GalaxyComponent::calculateArmValue( float rad,  QVector3D P,  float armStrength) {

    m_workRad = sqrt(rad);
    m_workTheta = -getTheta(P);
    m_workWinding = getWinding(rad);


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


float GalaxyComponent::getArm( float rad,  QVector3D p,  float disp) {
    float v = abs(findDifference(m_workWinding,m_workTheta + disp))/M_PI;
    return pow(1.0-v,m_componentParams.arm()*15);
}



float GalaxyComponent::getTheta( QVector3D p ) {
    QVector3D quatRot = m_currentGI->rotmat()*p;//rotmat.Mul(p, quatRot);
    return atan2(quatRot.x(), quatRot.z()) + m_componentParams.delta();
}


float GalaxyComponent::getWinding( float rad) {
    float r = rad + 0.05;
    float t = atan(exp(-0.25/(0.5*(r)))/m_galaxyParams->windingB())*2*m_galaxyParams->windingN();
    //float t = (*param.splineWinding)[r];
    float scale = 1.0;
    float t2 = 0.0;
    if (m_galaxyParams->innerTwirl() != 0) {
        t2 = -0.5/(pow(r,1.0)+0.1);
        scale = Util::smoothstep(0.00, m_galaxyParams->bulgeDust()*1.5f, r);
    }
    t = t*scale + t2*(1-scale);
    return t;

}

