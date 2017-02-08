#include "source/galaxy/spectrum.h"

QString ComponentSpectrum::name() const
{
return m_name;
}

QVector3D ComponentSpectrum::spectrum() const
{
return m_spectrum;
}

void ComponentSpectrum::setSpectrum(const QVector3D &spectrum)
{
m_spectrum = spectrum;
}

void ComponentSpectrum::setName(const QString &name)
{
m_name = name;
}

