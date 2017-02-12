#pragma once

#include "math.h"
#include <QVector3D>
#include <QDataStream>
#include "source/noise/noise.h"

class GalaxyParams {
public:
    QString m_name = "NewGalaxy";
    QVector3D m_axis = QVector3D(1,1,1);
    float m_bulgeDust = 0.025f;
    QVector3D m_bulgeAxis = QVector3D(1,1,1);

    float m_windingB = 0.5f;
    float m_windingN = 4;
    float m_noArms = 2;
    float m_arm1 = 0;
    float m_arm2 = M_PI;
    float m_arm3 = 2*M_PI;
    float m_arm4 = 3*M_PI;


    float m_innerTwirl = 0;

    float m_warpAmplitude;
    float m_warpScale;

    friend QDataStream& operator << ( QDataStream & s, const GalaxyParams& gp) {
        s << gp.m_name << gp.m_axis << gp.m_bulgeDust << gp.m_bulgeAxis << gp.m_windingB << gp.m_windingN;
        s << gp.m_noArms << gp.m_arm1 << gp.m_arm2 << gp.m_arm3 << gp.m_arm4;
        s << gp.m_innerTwirl << gp.m_warpAmplitude << gp.m_warpScale;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, GalaxyParams& gp) {
        s >> gp.m_name >> gp.m_axis >> gp.m_bulgeDust >> gp.m_bulgeAxis >> gp.m_windingB >> gp.m_windingN;
        s >> gp.m_noArms >> gp.m_arm1 >> gp.m_arm2 >> gp.m_arm3 >> gp.m_arm4;
        s >> gp.m_innerTwirl >> gp.m_warpAmplitude >> gp.m_warpScale;
        return s;
    }

private:

    int m_randShiftX, m_randShiftY;

    float m_diskStarModifier;
    Noise* m_noise = nullptr;


public:
    GalaxyParams();
    float bulgeDust() const;
    void setBulgeDust(float bulgeDust);
    QVector3D axis() const;
    void setAxis(const QVector3D &axis);
    float arm1() const;
    void setArm1(float arm1);
    float arm2() const;
    void setArm2(float arm2);
    float arm3() const;
    void setArm3(float arm3);
    float arm4() const;
    void setArm4(float arm4);
    float noArms() const;
    void setNoArms(float noArms);
    float windingB() const;
    void setWindingB(float windingB);
    float windingN() const;
    void setWindingN(float windingN);
    float innerTwirl() const;
    void setInnerTwirl(float innerTwirl);
    QVector3D bulgeAxis() const;
    void setBulgeAxis(const QVector3D &bulgeAxis);
    QString name() const;
    void setName(const QString &name);
    Noise *noise() const;
    void setNoise(Noise *noise);
};


