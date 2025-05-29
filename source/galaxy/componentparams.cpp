#include "source/galaxy/componentparams.h"

double ComponentParams::strength() const
{
    return m_strength;
}

void ComponentParams::setStrength(double strength)
{
    m_strength = strength;
}

double ComponentParams::arm() const
{
    return m_arm;
}

void ComponentParams::setArm(double arm)
{
    m_arm = arm;
}

double ComponentParams::z0() const
{
    return m_z0;
}

void ComponentParams::setZ0(double z0)
{
    m_z0 = z0;
}

double ComponentParams::r0() const
{
    return m_r0;
}

void ComponentParams::setR0(double r0)
{
    m_r0 = r0;
}

double ComponentParams::delta() const
{
    return m_delta;
}

void ComponentParams::setDelta(double delta)
{
    m_delta = delta;
}

double ComponentParams::winding() const
{
    return m_winding;
}

void ComponentParams::setWinding(double winding)
{
    m_winding = winding;
}

double ComponentParams::scale() const
{
    return m_scale;
}

void ComponentParams::setScale(double scale)
{
    m_scale = scale;
}

double ComponentParams::noiseOffset() const
{
    return m_noiseOffset;
}

void ComponentParams::setNoiseOffset(double noiseOffset)
{
    m_noiseOffset = noiseOffset;
}

double ComponentParams::noiseTilt() const
{
    return m_noiseTilt;
}

void ComponentParams::setNoiseTilt(double noiseTilt)
{
    m_noiseTilt = noiseTilt;
}

double ComponentParams::ks() const
{
    return m_ks;
}

void ComponentParams::setKs(double ks)
{
    m_ks = ks;
}

double ComponentParams::active() const
{
    return m_active;
}

void ComponentParams::setActive(double active)
{
    m_active = active;
}

QString ComponentParams::spectrum() const
{
    return m_spectrum;
}

void ComponentParams::setSpectrum(const QString &spectrum)
{
    m_spectrum = spectrum;
}

double ComponentParams::inner() const
{
    return m_inner;
}

void ComponentParams::setInner(double inner)
{
    m_inner = inner;
}

QString ComponentParams::name() const
{
    return m_name;
}

void ComponentParams::setName(const QString &name)
{
    m_name = name;
}

QString ComponentParams::className() const
{
    return m_className;
}

void ComponentParams::setClassName(const QString &className)
{
    m_className = className;
}
