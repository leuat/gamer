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

QString RenderingParams::currentGalaxy() const
{
    return m_currentGalaxy;
}

void RenderingParams::setCurrentGalaxy(const QString &currentGalaxy)
{
    m_currentGalaxy = currentGalaxy;
}

QString RenderingParams::sceneDirectory() const
{
    return m_sceneDirectory;
}

void RenderingParams::setSceneDirectory(const QString &sceneDirectory)
{
    m_sceneDirectory = sceneDirectory;
}

QString RenderingParams::galaxyDirectory() const
{
    return m_galaxyDirectory;
}

void RenderingParams::setGalaxyDirectory(const QString &galaxyDirectory)
{
    m_galaxyDirectory = galaxyDirectory;
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
//    qDebug() << "TAR" << camera().target();
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }
    QDataStream in(&file);

    in >> *this;

    file.close();
}


int RenderingParams::previewSize() const
{
    return m_previewSize;
}

void RenderingParams::setPreviewSize(int previewSize)
{
    m_previewSize = previewSize;
}
