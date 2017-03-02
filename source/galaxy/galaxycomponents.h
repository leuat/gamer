#pragma once

#include <QVector3D>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

class GalaxyComponentBulge : GalaxyComponent {
public:
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;

    float calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, float weight) override;
    float getHeightModulation(float height) override;
    float getRadius(const QVector3D& p, QVector3D& P, float& dott, GalaxyInstance* gi);

};

class GalaxyComponentDisk : GalaxyComponent {
	
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;
};

class GalaxyComponentDust : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;

};

class GalaxyComponentStars : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;
		
};

class GalaxyComponentStarsSmall : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;

};

class GalaxyComponentDust2 : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;

};

class GalaxyComponentDustPositive : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;

};
