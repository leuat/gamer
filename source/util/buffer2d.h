#pragma once

#include <QVector>
#include <QVector3D>
#include <QImage>
#include <QRgb>
#ifdef USE_HEALPIX
#include <healpix_map.h>
#endif


class Buffer2D
{
    QVector<QVector3D> m_buffer;
    int m_size;

    QColor PostProcess(const QVector3D& v, double exposure, double gamma, double saturation);

public:
    ~Buffer2D();
    Buffer2D();
    Buffer2D(int size);
    void Initialize(int size);

    void Set(const int i, const int j, const QVector3D& v);
    void Set(const int i, const QVector3D& v);
    QVector3D Get(const int i, const int j);
    QVector3D Get(const int i);

    void DrawBox(Buffer2D* backImage, const int i, const int j, const int size, QVector3D val);
    void ToColorBuffer(QImage* image, QImage* shadowImage, double exposure, double gamma, double saturation);
    void ToColorBuffer(QImage* image, double exposure, double gamma, double saturation);
    void fill(const QVector3D v);
    void SaveFits(QString filename);
    void RenderStars(int noStars, int baseSize, int sizeSpread, double strength);
    QByteArray* toQByteArray(int no);

#ifdef USE_HEALPIX
    void MollweideProjection(Healpix_Map<double>& map);
#endif

    void Add(Buffer2D* other);
    void CopyTo(Buffer2D* to);

    void RenderGaussian(int i, int j, int w, QVector3D cs);


    int size() const;
};

