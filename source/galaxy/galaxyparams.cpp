#include "source/galaxy/galaxyparams.h"

float GalaxyParams::bulgeDust() const
{
    return m_bulgeDust;
}

void GalaxyParams::setBulgeDust(float bulgeDust)
{
    m_bulgeDust = bulgeDust;
}

QVector3D GalaxyParams::axis() const
{
    return m_axis;
}

void GalaxyParams::setAxis(const QVector3D &axis)
{
    m_axis = axis;
}

float GalaxyParams::arm1() const
{
    return m_arm1;
}

void GalaxyParams::setArm1(float arm1)
{
    m_arm1 = arm1;
}

float GalaxyParams::arm2() const
{
    return m_arm2;
}

void GalaxyParams::setArm2(float arm2)
{
    m_arm2 = arm2;
}

float GalaxyParams::arm3() const
{
    return m_arm3;
}

void GalaxyParams::setArm3(float arm3)
{
    m_arm3 = arm3;
}

float GalaxyParams::arm4() const
{
    return m_arm4;
}

void GalaxyParams::setArm4(float arm4)
{
    m_arm4 = arm4;
}

float GalaxyParams::noArms() const
{
    return m_noArms;
}

void GalaxyParams::setNoArms(float noArms)
{
    m_noArms = noArms;
}

float GalaxyParams::windingB() const
{
    return m_windingB;
}

void GalaxyParams::setWindingB(float windingB)
{
    m_windingB = windingB;
}

float GalaxyParams::windingN() const
{
    return m_windingN;
}

void GalaxyParams::setWindingN(float windingN)
{
    m_windingN = windingN;
}

float GalaxyParams::innerTwirl() const
{
    return m_innerTwirl;
}

void GalaxyParams::setInnerTwirl(float innerTwirl)
{
    m_innerTwirl = innerTwirl;
}

QVector3D GalaxyParams::bulgeAxis() const
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

GalaxyParams::GalaxyParams() {
    m_noArms = 2;
    m_arm1 = 0;
    m_arm2 = M_PI;
    m_arm3 = M_PI/2.0;
    m_arm4 = M_PI + M_PI/2.0;

    m_randShiftX = 1;//(int)((Random.value*1001.1f));
    m_randShiftY = 2;//(int)((Random.value*1001.9f));
}
