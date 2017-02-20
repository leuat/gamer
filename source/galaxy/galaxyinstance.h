#pragma once

#include <QVector>
#include <QVector3D>
#include <QQuaternion>
#include "source/galaxy/galaxy.h"

class GalaxyInstance {
private:
    Galaxy* m_galaxy;
    float m_intensityScale=1, m_redshift=1;
    QVector3D m_position = QVector3D(0,0,0);
    QVector3D m_orientation = QVector3D(0,1,0);
    QQuaternion m_rotmat;
    QString m_galaxyName;

public:
    Galaxy* GetGalaxy() {
        return m_galaxy;
    }

    float m_workTheta, m_workRad, m_workWinding, m_workLength;
    float m_winding;
    float m_currentRadius;

    void SetGalaxy(Galaxy* g);
    GalaxyInstance();
    GalaxyInstance* Clone();
    void setupQuaternions();
    GalaxyInstance(Galaxy* g, QString n, QVector3D p, QVector3D o, float ins, float rs);

    float intensityScale() const;
    void setIntensityScale(float intensityScale);
    float redshift() const;
    void setRedshift(float redshift);
    QQuaternion& rotmat();
    void setRotmat(const QQuaternion &rotmat);
    QVector3D position() const;
    void setPosition(const QVector3D &position);
    QVector3D orientation() const;
    void setOrientation(const QVector3D &orientation);



};
