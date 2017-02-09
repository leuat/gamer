#include "source/galaxy/galaxycomponents.h"
#include "math.h"

void GalaxyComponentBulge::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    float rho_0 = m_componentParams.strength()*ival;
    QVector3D pos = m_currentGI->rotmat()*(p);

    float rad = (pos.length()+0.01f)*m_componentParams.r0();
    rad+=0.01f;
    float i = rho_0 * (pow(rad,-0.855)*exp(-pow(rad,1/4.0f)) -0.05f) *m_currentGI->intensityScale();


    if (i<0) i=0;
    rp->setI(rp->I() + i*m_spectrum->spectrum()*rp->scale);

}

void GalaxyComponentBulge::calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, float weight) {
    m_currentGI = gi;
    componentIntensity(rp, p, weight);
}



void GalaxyComponentDisk::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    float p2 = 0.5;
    if (ival<0.0005)
        return;
    p2 =abs(getPerlinCloudNoise(p, rp->winding, 9, m_componentParams.scale(), m_componentParams.ks()));

    p2 = fmax (p2, 0.01f);
    if (p2!=0)
        p2=0.1/p2;

    p2 = pow(p2,m_componentParams.noiseTilt());

    p2+=m_componentParams.noiseOffset();

    if (p2<0)
        return;

    rp->setI( rp->I() + ival*p2*m_spectrum->spectrum()*rp->getScale());
}




void GalaxyComponentDust::componentIntensity(RasterPixel* rp, QVector3D& p, float ival) {
    if (ival<0.0005)
        return;

    float p2 = getPerlinCloudNoise(p, rp->winding, 10, m_componentParams.scale(), m_componentParams.ks());
    //if (p2!=0)
    p2=0.1f/(abs(p2) + 0.1f);

    p2 = p2 + m_componentParams.noiseOffset();
    if (p2<0)
        return;
    p2 = pow(5*p2,m_componentParams.noiseTilt());

    float s = 0.01;

    rp->I().setX(rp->I().x()*exp(-p2*ival*m_spectrum->spectrum().x()*s));
    rp->I().setY(rp->I().y()*exp(-p2*ival*m_spectrum->spectrum().y()*s));
    rp->I().setZ(rp->I().z()*exp(-p2*ival*m_spectrum->spectrum().z()*s));


}

void GalaxyComponentStars::componentIntensity(RasterPixel* rp, QVector3D& r, float ival) {
    /*	float perlinnoise = Mathf.Abs(Util.Simplex.octave_noise_3d(14,componentParams.ks,0.01f*componentParams.scale,r.x,r.y,r.z));

            float addNoise = 0;
            if (componentParams.noiseOffset != 0)
            {
                addNoise = (componentParams.noiseOffset * getPerlinCloudNoise(r * 2, currentRadius, winding, 4, 2, -2));
                addNoise += 0.5f * (componentParams.noiseOffset * getPerlinCloudNoise(r * 2, currentRadius, winding * 0.5f, 4, 4, -2));
            }
            float val = Mathf.Abs(Mathf.Pow(perlinnoise+1 + addNoise,componentParams.noiseTilt));
            //            val = perlinnoise;

            rp.I +=  ival * val * spectrum.spectrum *rp.scale;


        }
    */

}





