#pragma once

#include <QString>
#include <QVector3D>
#include <QDataStream>

class ComponentParams {
public:
    QString m_className = "bulge";
    QString m_spectrum = "White";
    QString m_name = "New component";
    double m_strength = 1;
    double m_arm = 0.5;
    double m_z0 = 0.02;
    double m_r0 = 0.5;
    double m_inner = 0.0;
    double m_active = 1;
    double m_delta = 0;
    double m_winding = 0.1;
    double m_scale = 1;
    double m_noiseOffset = 0;
    double m_noiseTilt = 1;
    double m_ks = 1;






public:

    friend QDataStream& operator << ( QDataStream & s, const ComponentParams& cp) {
        s << cp.m_className  << cp.m_strength << cp.m_spectrum << cp.m_arm << cp.m_z0 << cp.m_r0;
        s << cp.m_active << cp.m_delta << cp.m_winding << cp.m_scale << cp.m_noiseOffset << cp.m_noiseTilt;
        s << cp.m_ks << cp.m_inner << cp.m_name;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, ComponentParams& cp) {
        s >> cp.m_className  >> cp.m_strength >> cp.m_spectrum >> cp.m_arm >> cp.m_z0 >> cp.m_r0;
        s >> cp.m_active >> cp.m_delta >> cp.m_winding >> cp.m_scale >> cp.m_noiseOffset >> cp.m_noiseTilt;
        s >> cp.m_ks >> cp.m_inner >> cp.m_name;
        return s;
    }

    ComponentParams(double s, double a, double z, double r0) { //, CGraph* g) {
        m_strength = s;
        m_arm = a;
        m_z0 = z;
        m_r0 = r0;
    }


    ComponentParams() {
        m_arm = 1.0f;
        m_strength = 1.0f;
        m_z0 = 0.02f;
        m_className = "Bulge";
    }
    QString className() const;
    void setClassName(const QString &className);
    double strength() const;
    void setStrength(double strength);
    double arm() const;
    void setArm(double arm);
    double z0() const;
    void setZ0(double z0);
    double r0() const;
    void setR0(double r0);
    double delta() const;
    void setDelta(double delta);
    double winding() const;
    void setWinding(double winding);
    double scale() const;
    void setScale(double scale);
    double noiseOffset() const;
    void setNoiseOffset(double noiseOffset);
    double noiseTilt() const;
    void setNoiseTilt(double noiseTilt);
    double ks() const;
    void setKs(double ks);
    double active() const;
    void setActive(double active);
    QString spectrum() const;
    void setSpectrum(const QString &spectrum);
    double inner() const;
    void setInner(double inner);
    QString name() const;
    void setName(const QString &name);
};


