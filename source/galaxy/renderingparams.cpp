#include "source/galaxy/renderingparams.h"
#include <QFile>

int RenderingParams::size() const
{
    return m_size;
}

void RenderingParams::setSize(int size)
{
    m_size = size;
}

GamerCamera  &RenderingParams::camera()
{
    return m_camera;
}

void RenderingParams::setCamera(const GamerCamera &camera)
{
    m_camera = camera;
}

float RenderingParams::rayStep() const
{
    return m_rayStep;
}

void RenderingParams::setRayStep(float rayStep)
{
    m_rayStep = rayStep;
}

void RenderingParams::Save(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open " << filename;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_6);
    out << *this;
    file.flush();
    file.close();

}

void RenderingParams::Load(QString filename)
{
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);

    in >> *this;

    file.close();
}

