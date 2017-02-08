#include "source/galaxy/componentparams.h"

float ComponentParams::strength() const
{
    return m_strength;
}

void ComponentParams::setStrength(float strength)
{
    m_strength = strength;
}

float ComponentParams::arm() const
{
return m_arm;
}

void ComponentParams::setArm(float arm)
{
m_arm = arm;
}

float ComponentParams::z0() const
{
return m_z0;
}

void ComponentParams::setZ0(float z0)
{
m_z0 = z0;
}

float ComponentParams::r0() const
{
return m_r0;
}

void ComponentParams::setR0(float r0)
{
m_r0 = r0;
}

float ComponentParams::delta() const
{
return m_delta;
}

void ComponentParams::setDelta(float delta)
{
m_delta = delta;
}

float ComponentParams::winding() const
{
return m_winding;
}

void ComponentParams::setWinding(float winding)
{
m_winding = winding;
}

float ComponentParams::scale() const
{
return m_scale;
}

void ComponentParams::setScale(float scale)
{
m_scale = scale;
}

float ComponentParams::noiseOffset() const
{
return m_noiseOffset;
}

void ComponentParams::setNoiseOffset(float noiseOffset)
{
m_noiseOffset = noiseOffset;
}

float ComponentParams::noiseTilt() const
{
return m_noiseTilt;
}

void ComponentParams::setNoiseTilt(float noiseTilt)
{
m_noiseTilt = noiseTilt;
}

float ComponentParams::ks() const
{
return m_ks;
}

void ComponentParams::setKs(float ks)
{
m_ks = ks;
}

float ComponentParams::active() const
{
return m_active;
}

void ComponentParams::setActive(float active)
{
m_active = active;
}

QString ComponentParams::className() const
{
    return m_className;
}

void ComponentParams::setClassName(const QString &className)
{
    m_className = className;
}
