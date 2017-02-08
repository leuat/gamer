#include "rasterpixel.h"

QVector3D& RasterPixel::I()
{
    return m_I;
}

void RasterPixel::setI(const QVector3D &I)
{
    m_I = I;
}

void RasterPixel::Add(RasterPixel* o) {
    m_I += o->I();
}

float RasterPixel::getScale() const
{
    return scale;
}

void RasterPixel::setScale(float value)
{
    scale = value;
}

RasterPixel::RasterPixel(RasterPixel* o) {
    m_I = o->I();
}

RasterPixel::RasterPixel() {
}

void RasterPixel::Floor(float f) {
    if (m_I.x()<f) m_I.setX(f);
    if (m_I.y()<f) m_I.setY(f);
    if (m_I.z()<f) m_I.setZ(f);
}
