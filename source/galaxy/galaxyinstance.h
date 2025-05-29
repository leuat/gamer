#pragma once

#include <QVector>
#include <QVector3D>
#include <QQuaternion>
#include "source/galaxy/galaxy.h"

class GalaxyInstance {
private:
    Galaxy* m_galaxy;
    double m_intensityScale=1, m_redshift=1;
    QVector3D m_position = QVector3D(0,0,0);
    QVector3D m_orientation = QVector3D(0,1,0);
    QQuaternion m_rotmat;
    QString m_galaxyName;

public:
    Galaxy* GetGalaxy() {
        return m_galaxy;
    }

    double m_workTheta, m_workRad, m_workWinding, m_workLength;
    double m_winding;
    double m_currentRadius;

    void SetGalaxy(Galaxy* g);
    GalaxyInstance();
    GalaxyInstance* Clone();
    void setupQuaternions();
    GalaxyInstance(Galaxy* g, QString n, QVector3D p, QVector3D o, double ins, double rs);

    double intensityScale() const;
    void setIntensityScale(double intensityScale);
    double redshift() const;
    void setRedshift(double redshift);
    QQuaternion& rotmat();
    void setRotmat(const QQuaternion &rotmat);
    QVector3D position() const;
    void setPosition(const QVector3D &position);
    QVector3D& orientation();
    void setOrientation(const QVector3D &orientation);



};
