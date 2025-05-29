#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "math.h"
#include <QVector3D>
#include <QDataStream>
#include "source/noise/noise.h"


class GalaxyParams {
public:
    QString m_name = "NewGalaxy";
    QVector3D m_axis = QVector3D(1,1,1);
    double m_bulgeDust = 0.025f;
    QVector3D m_bulgeAxis = QVector3D(1,1,1);

    double m_windingB = 0.5f;
    double m_windingN = 4;
    double m_noArms = 2;
    double m_arm1 = 0;
    double m_arm2 = M_PI;
    double m_arm3 = 2*M_PI;
    double m_arm4 = 3*M_PI;


    double m_innerTwirl = 0;

    double m_warpAmplitude;
    double m_warpScale;

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

    double m_diskStarModifier;
    Noise* m_noise = nullptr;


public:
    GalaxyParams();
    double bulgeDust() const;
    void setBulgeDust(double bulgeDust);
    QVector3D& axis();
    void setAxis(const QVector3D &axis);
    double arm1() const;
    void setArm1(double arm1);
    double arm2() const;
    void setArm2(double arm2);
    double arm3() const;
    void setArm3(double arm3);
    double arm4() const;
    void setArm4(double arm4);
    double noArms() const;
    void setNoArms(double noArms);
    double windingB() const;
    void setWindingB(double windingB);
    double windingN() const;
    void setWindingN(double windingN);
    double innerTwirl() const;
    void setInnerTwirl(double innerTwirl);
    QVector3D& bulgeAxis();
    void setBulgeAxis(const QVector3D &bulgeAxis);
    QString name() const;
    void setName(const QString &name);
    Noise *noise() const;
    void setNoise(Noise *noise);
};


