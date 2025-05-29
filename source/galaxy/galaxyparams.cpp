#include "source/galaxy/galaxyparams.h"

double GalaxyParams::bulgeDust() const
{
    return m_bulgeDust;
}

void GalaxyParams::setBulgeDust(double bulgeDust)
{
    m_bulgeDust = bulgeDust;
}

QVector3D& GalaxyParams::axis()
{
    return m_axis;
}

void GalaxyParams::setAxis(const QVector3D &axis)
{
    m_axis = axis;
}

double GalaxyParams::arm1() const
{
    return m_arm1;
}

void GalaxyParams::setArm1(double arm1)
{
    m_arm1 = arm1;
}

double GalaxyParams::arm2() const
{
    return m_arm2;
}

void GalaxyParams::setArm2(double arm2)
{
    m_arm2 = arm2;
}

double GalaxyParams::arm3() const
{
    return m_arm3;
}

void GalaxyParams::setArm3(double arm3)
{
    m_arm3 = arm3;
}

double GalaxyParams::arm4() const
{
    return m_arm4;
}

void GalaxyParams::setArm4(double arm4)
{
    m_arm4 = arm4;
}

double GalaxyParams::noArms() const
{
    return m_noArms;
}

void GalaxyParams::setNoArms(double noArms)
{
    m_noArms = noArms;
}

double GalaxyParams::windingB() const
{
    return m_windingB;
}

void GalaxyParams::setWindingB(double windingB)
{
    m_windingB = windingB;
}

double GalaxyParams::windingN() const
{
    return m_windingN;
}

void GalaxyParams::setWindingN(double windingN)
{
    m_windingN = windingN;
}

double GalaxyParams::innerTwirl() const
{
    return m_innerTwirl;
}

void GalaxyParams::setInnerTwirl(double innerTwirl)
{
    m_innerTwirl = innerTwirl;
}

QVector3D& GalaxyParams::bulgeAxis()
{
    return m_bulgeAxis;
}

void GalaxyParams::setBulgeAxis(const QVector3D &bulgeAxis)
{
    m_bulgeAxis = bulgeAxis;
}

QString GalaxyParams::name() const
{
    return m_name;
}

void GalaxyParams::setName(const QString &name)
{
    m_name = name;
}

Noise *GalaxyParams::noise() const
{
    return m_noise;
}

void GalaxyParams::setNoise(Noise *noise)
{
    m_noise = noise;
}

GalaxyParams::GalaxyParams() {
    m_noArms = 2;
    m_arm1 = 0;
    m_arm2 = M_PI;
    m_arm3 = M_PI/2.0;
    m_arm4 = M_PI + M_PI/2.0;

    m_randShiftX = rand()%1024;
    m_randShiftY = rand()%1023;
}
