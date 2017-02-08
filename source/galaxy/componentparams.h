#pragma once

#include <QString>
#include <QVector3D>
#include <QDataStream>

class ComponentParams {
public:
    QString m_className = "bulge";
    QString m_spectrum = "White";
    float m_strength = 1;
    float m_arm = 0.5;
    float m_z0 = 0.02;
    float m_r0 = 0.5;
    float m_active = 1;
    float m_delta = 0;
    float m_winding = 0.1;
    float m_scale = 1;
    float m_noiseOffset = 0;
    float m_noiseTilt = 1;
    float m_ks = 1;

public:

    friend QDataStream& operator << ( QDataStream & s, const ComponentParams& cp) {
        s << cp.m_className  << cp.m_strength << cp.m_spectrum << cp.m_arm << cp.m_z0 << cp.m_r0;
        s << cp.m_active << cp.m_delta << cp.m_winding << cp.m_scale << cp.m_noiseOffset << cp.m_noiseTilt;
        s << cp.m_ks;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, ComponentParams& cp) {
        s >> cp.m_className  >> cp.m_strength >> cp.m_spectrum >> cp.m_arm >> cp.m_z0 >> cp.m_r0;
        s >> cp.m_active >> cp.m_delta >> cp.m_winding >> cp.m_scale >> cp.m_noiseOffset >> cp.m_noiseTilt;
        s >> cp.m_ks;
        return s;
    }

    ComponentParams(float s, float a, float z, float r0, float winding) { //, CGraph* g) {
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
    float strength() const;
    void setStrength(float strength);
    float arm() const;
    void setArm(float arm);
    float z0() const;
    void setZ0(float z0);
    float r0() const;
    void setR0(float r0);
    float delta() const;
    void setDelta(float delta);
    float winding() const;
    void setWinding(float winding);
    float scale() const;
    void setScale(float scale);
    float noiseOffset() const;
    void setNoiseOffset(float noiseOffset);
    float noiseTilt() const;
    void setNoiseTilt(float noiseTilt);
    float ks() const;
    void setKs(float ks);
    float active() const;
    void setActive(float active);
    QString spectrum() const;
    void setSpectrum(const QString &spectrum);
};


