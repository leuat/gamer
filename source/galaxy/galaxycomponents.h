#pragma once

#include <QVector3D>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

class GalaxyComponentBulge : GalaxyComponent {
public:
    void componentIntensity(RasterPixel* rp, QVector3D& p, float ival) override;

    float calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, float weight) override;
    float getHeightModulation(float height) override;

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

class GalaxyComponentDust2 : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, float ival) override;

};
