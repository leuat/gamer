#include "source/galaxy/renderingparams.h"
#include <QFile>
#include "source/util/util.h"

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

double RenderingParams::rayStep() const
{
    return m_rayStep;
}

void RenderingParams::setRayStep(double rayStep)
{
    m_rayStep = rayStep;
}

QString RenderingParams::renderType() const
{
    return m_renderType;
}

void RenderingParams::setRenderType(const QString &renderType)
{
    m_renderType = renderType;
}

int RenderingParams::nside() const
{
    return m_nside;
}

void RenderingParams::setNside(int nside)
{
    m_nside = nside;
}

Spectra& RenderingParams::spectra()
{
    return m_spectra;
}

QString RenderingParams::imageDirectory() const
{
    return m_imageDirectory;
}

void RenderingParams::setImageDirectory(const QString &imageDirectory)
{
    m_imageDirectory = imageDirectory;
}

double RenderingParams::starStrength() const
{
    return m_starStrength;
}

void RenderingParams::setStarStrength(double starStrength)
{
    m_starStrength = starStrength;
}

double RenderingParams::starSizeSpread() const
{
    return m_starSizeSpread;
}

void RenderingParams::setStarSizeSpread(double starSizeSpread)
{
    m_starSizeSpread = starSizeSpread;
}

double RenderingParams::starSize() const
{
    return m_starSize;
}

void RenderingParams::setStarSize(double starSize)
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

double RenderingParams::saturation() const
{
    return m_saturation;
}

void RenderingParams::setSaturation(double saturation)
{
    m_saturation = saturation;
}

double RenderingParams::gamma() const
{
    return m_gamma;
}

void RenderingParams::setGamma(double gamma)
{
    m_gamma = gamma;
}

double RenderingParams::exposure() const
{
    return m_exposure;
}

void RenderingParams::setExposure(double exposure)
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
    QFile file(Util::path + filename);
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
    QFile file(Util::path + filename);
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
