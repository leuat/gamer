#pragma once

#include <QVector3D>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

class GalaxyComponentBulge : GalaxyComponent {
public:
    void componentIntensity(RasterPixel* rp, QVector3D& p, double ival) override;

    double calculateIntensity(RasterPixel* rp, QVector3D& p, GalaxyInstance* gi, double weight) override;
    double getHeightModulation(double height) override;
    double getRadius(const QVector3D& p, QVector3D& P, double& dott, GalaxyInstance* gi);

};

class GalaxyComponentDisk : GalaxyComponent {
	
    void componentIntensity(RasterPixel* rp, QVector3D& p, double ival) override;
};

class GalaxyComponentDust : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& p, double ival) override;

};

class GalaxyComponentStars : GalaxyComponent {
		
    void componentIntensity(RasterPixel* rp, QVector3D& r, double ival) override;
		
};

class GalaxyComponentStarsSmall : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, double ival) override;

};

class GalaxyComponentDust2 : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, double ival) override;

};

class GalaxyComponentDustPositive : GalaxyComponent {

    void componentIntensity(RasterPixel* rp, QVector3D& r, double ival) override;

};
