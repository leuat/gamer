#include "buffer2d.h"
#include "source/util/util.h"
#include "source/util/gmessages.h"
Buffer2D::Buffer2D()
{

}

Buffer2D::Buffer2D(int size)
{
    Initialize(size);
}

void Buffer2D::Initialize(int size)
{
    m_buffer.resize(size*size);
    m_size = size;
    for (int i=0;i<size*size;i++)
        m_buffer[i] = QVector3D(0,0,0);
}

void Buffer2D::Set(const int i, const int j, const QVector3D &v)
{
    int x = Util::clamp(i,0,m_size);
    int y = Util::clamp(j,0,m_size);
    m_buffer[ x + y*m_size] = v;
}

QVector3D Buffer2D::Get(const int i, const int j)
{
    int x = Util::clamp(i,0,m_size);
    int y = Util::clamp(j,0,m_size);
    return m_buffer[ x + y*m_size];
}

QVector3D Buffer2D::Get(const int i)
{
    int x = Util::clamp(i,0,m_size*m_size);
    return m_buffer[ x ];
}


void Buffer2D::DrawBox(Buffer2D* backImage,  const int i, const int j, const int size, QVector3D val) {
    QVector3D mark = QVector3D(1,1,1);
    for (int x=max(0, i-size/2);x<=min(m_size-1, i+size/2);x++)
        for (int y=max(0, j-size/2);y<=min(m_size-1, j+size/2);y++) {
            QVector3D refval = backImage->Get(x,y);
            if (refval.x()==0) {
                Set(x,y,val);
                if (x==i && y== j)
                    backImage->Set(x,y,mark);
            }
        }
}

void Buffer2D::ToColorBuffer(QImage *image, QImage* shadowImage, float exposure, float gamma, float saturation)
{
    if (image->width()!=m_size || shadowImage->width() != m_size) {
        qDebug() << "ERROR! Color buffer and render buffer not equal size!";
        qDebug() << "Main: " << m_size;
        qDebug() << "Image: " << image->width();
        qDebug() << "ShadowImage: " << shadowImage->width();
        return;
    }


#pragma omp for
    for (int i=0;i<m_size;i++) {
        for (int j=0;j<m_size;j++) {
            QColor c = PostProcess(Get(i,j), exposure, gamma, saturation);
            QColor c2 = QColor(c.blue(), c.green(), c.red());
            image->setPixel(i,j,c.rgb());
            shadowImage->setPixel(i,j,c2.rgb());
        }
    }
}

int Buffer2D::size() const
{
    return m_size;
}

QColor Buffer2D::PostProcess(const QVector3D &val, float exposure, float gamma, float saturation)
{
    QVector3D v = val;

    v *= 1.0/exposure;
    v.setX(pow(v.x(), gamma));
    v.setY(pow(v.y(), gamma));
    v.setZ(pow(v.z(), gamma));

    float center = (v.x() + v.y() + v.z())/3.0;
    QVector3D tmp (center-v.x(), center-v.y(), center-v.z());
    v.setX(center - saturation*tmp.x());
    v.setY(center - saturation*tmp.y());
    v.setZ(center - saturation*tmp.z());


     float s = 10;
     QVector3D c = Util::clamp(v*s, 0, 255);
     return QColor(c.z(), c.y(),c.x());

}

void Buffer2D::fill(const QVector3D v)
{
    for (int i=0;i<m_buffer.size();i++)
        m_buffer[i] = v;
}

QByteArray *Buffer2D::toQByteArray(int no)
{
    QByteArray* ba = new QByteArray();
    for (int i=0;i<m_size;i++)
        for (int j=0;j<m_size;j++) {
            float val = Get(j, m_size-1-i)[no];
            ba->append((const char*)(&val), sizeof(float));
        }
   return ba;

}
