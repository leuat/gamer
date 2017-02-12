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

QString RenderingParams::imageDirectory() const
{
    return m_imageDirectory;
}

void RenderingParams::setImageDirectory(const QString &imageDirectory)
{
    m_imageDirectory = imageDirectory;
}

float RenderingParams::starStrength() const
{
    return m_starStrength;
}

void RenderingParams::setStarStrength(float starStrength)
{
    m_starStrength = starStrength;
}

float RenderingParams::starSizeSpread() const
{
    return m_starSizeSpread;
}

void RenderingParams::setStarSizeSpread(float starSizeSpread)
{
    m_starSizeSpread = starSizeSpread;
}

float RenderingParams::starSize() const
{
    return m_starSize;
}

void RenderingParams::setStarSize(float starSize)
{
    m_starSize = starSize;
}

int RenderingParams::noStars() const
{
    return m_noStars;
}

void RenderingParams::setNoStars(int noStars)
{
    m_noStars = noStars;
}

float RenderingParams::saturation() const
{
    return m_saturation;
}

void RenderingParams::setSaturation(float saturation)
{
    m_saturation = saturation;
}

float RenderingParams::gamma() const
{
    return m_gamma;
}

void RenderingParams::setGamma(float gamma)
{
    m_gamma = gamma;
}

float RenderingParams::exposure() const
{
    return m_exposure;
}

void RenderingParams::setExposure(float exposure)
{
    m_exposure = exposure;
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
