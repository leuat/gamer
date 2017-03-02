#pragma once

#include <QVector3D>

class RasterPixel {
private:
    QVector3D m_I = QVector3D(0,0,0);

public:
    float winding, radius;
    float z;
    float tmp;
    QVector3D dir;
    float step;
    QVector3D P;
    QVector3D dust = QVector3D(0,0,0);

    void Add(RasterPixel* o);
    float scale = 1;
    RasterPixel(RasterPixel* o);
    RasterPixel();
    void Floor(float f);
    QVector3D& I();
    void setI(const QVector3D &I);
    float getScale() const;
    void setScale(float value);
};
