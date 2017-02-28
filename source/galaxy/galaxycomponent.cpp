#include "source/galaxy/galaxycomponent.h"
#include "source/util/util.h"

ComponentParams& GalaxyComponent::getComponentParams()
{
    return m_componentParams;
}

ComponentSpectrum* GalaxyComponent::getSpectrum() const
{
    return m_spectrum;
}

void GalaxyComponent::setSpectrum(ComponentSpectrum* spectrum)
{
    m_spectrum = spectrum;
}

GalaxyComponent::GalaxyComponent() {
    
}


void GalaxyComponent::Initialize(ComponentParams* cp, GalaxyParams* gp) {
    m_componentParams = *cp;
    m_galaxyParams = gp;

}


void GalaxyComponent::componentIntensity(RasterPixel* rp, QVector3D& p, float ival )  {
    qDebug() << "GALAXYCOMPONENT: COMPONENTINTENSITY SHOULD NEVER BE CALLED";
    // Dummy stuff to ignore warnings
    rp->setI(p);
    rp->I().setX(ival);
}

void GalaxyComponent::setGalaxyParam(GalaxyParams* gp) {
    m_galaxyParams = gp;
}




float GalaxyComponent::calculateIntensity(RasterPixel* rp, QVector3D& p,
                                         GalaxyInstance* gi, const float weight) {
    QVector3D P;
    m_currentGI = gi;
//    float z = 1;
//    float m_currentRadius;
//    m_currentRadius = getRadius(p, P, z, gi);
//    z = getHeightModulation(z);
//    float z = rp->z;
//    float m_currentRadius = rp->radius;

    float armVal = 1;
    float m_winding = 0;
    if (rp->z>0.01)
    {

        float intensity = getRadialIntensity(rp->radius);
        if (intensity>0.1) intensity = 0.1;
        if (intensity >0.001) {

            float scale = 1;
            scale = pow(Util::smoothstep(0, 1.0f*m_componentParams.inner(), rp->radius), 4);

            if (m_componentParams.arm()!=0) {
                armVal = calculateArmValue(rp->radius, rp->P);

                if (m_componentParams.winding()!=0)
                    m_winding = getWinding(rp->radius)*m_componentParams.winding();///(rad+1.0);

            }
            rp->winding = m_winding;
//            rp->radius = m_currentRadius;

            // equation 5 from the paper
            float val = (m_componentParams.strength())*scale*armVal*rp->z*intensity*gi->intensityScale();
            if (val * weight > 0.0005) {
                componentIntensity(rp, p, val * weight);
                return val;
            }
            else return 0;
        }
    }
    else return 0;
}

