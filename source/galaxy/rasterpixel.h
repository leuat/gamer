#pragma once

#include <QVector3D>

class RasterPixel {
private:
    QVector3D m_I = QVector3D(0,0,0);

public:
    double winding, radius;
    double z;
    double tmp;
    QVector3D dir;
    double step;
    QVector3D P;
    QVector3D dust = QVector3D(0,0,0);

    void Add(RasterPixel* o);
    double scale = 1;
    RasterPixel(RasterPixel* o);
    RasterPixel();
    void Floor(double f);
    QVector3D& I();
    void setI(const QVector3D &I);
    double getScale() const;
    void setScale(double value);
};
