#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/galaxy.h"

GalaxyInstance::GalaxyInstance() {

}

void GalaxyInstance::SetGalaxy(Galaxy* g) {
    m_galaxy = g;
    m_galaxyName = m_galaxy->galaxyParams().name(); }

QVector3D& GalaxyInstance::orientation()
{
    return m_orientation;
}

void GalaxyInstance::setOrientation(const QVector3D &orientation)
{
    m_orientation = orientation;
}

QVector3D GalaxyInstance::position() const
{
    return m_position;
}

void GalaxyInstance::setPosition(const QVector3D &position)
{
    m_position = position;
}

QQuaternion& GalaxyInstance::rotmat()
{
    return m_rotmat;
}

void GalaxyInstance::setRotmat(const QQuaternion &rotmat)
{
    m_rotmat = rotmat;
}

double GalaxyInstance::redshift() const
{
    return m_redshift;
}

void GalaxyInstance::setRedshift(double redshift)
{
    m_redshift = redshift;
}

double GalaxyInstance::intensityScale() const
{
    return m_intensityScale;
}

void GalaxyInstance::setIntensityScale(double intensityScale)
{
    m_intensityScale = intensityScale;
}


GalaxyInstance* GalaxyInstance::Clone() {
    Galaxy* g = m_galaxy->Clone();
    return new GalaxyInstance(g,m_galaxyName, m_position, m_orientation, m_intensityScale, m_redshift);
}


void GalaxyInstance::setupQuaternions() {
    m_rotmat = QQuaternion::rotationTo(QVector3D(0,1,0),m_orientation);
}

GalaxyInstance::GalaxyInstance(Galaxy* g, QString n, QVector3D p, QVector3D o, double ins, double rs) {
    m_galaxy = g;
    m_position = p;
    m_galaxyName = n;
    m_orientation = o;
    m_redshift = rs;
    m_intensityScale = ins;
    setupQuaternions();
}


