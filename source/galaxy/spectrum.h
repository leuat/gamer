#pragma once

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QDataStream>
#include <QDebug>

class ComponentSpectrum {
public:
    QString m_name ="None";
    QVector3D m_spectrum = QVector3D(0,0,0);
public:
    ComponentSpectrum() {

    }

    ComponentSpectrum(QVector3D s, QString n) {
        m_name = n;
        m_spectrum = s;
    }

    friend QDataStream& operator << ( QDataStream & s, const ComponentSpectrum& cs) {
        s << cs.m_name << cs.m_spectrum;
        return s;
    }
    friend QDataStream& operator >> ( QDataStream & s, ComponentSpectrum& cs) {
        s >> cs.m_name >> cs.m_spectrum;
        return s;
    }


    QString name() const;
    void setName(const QString &name);
    QVector3D spectrum() const;
    void setSpectrum(const QVector3D &spectrum);
};

class Spectra  {


public:
    QVector<ComponentSpectrum*> m_spectra;
    ComponentSpectrum* m_default = nullptr;

    Spectra() {
        m_default = new ComponentSpectrum(QVector3D(1,1,1), "default");
    }

    void PopulateSpectra() {
        m_spectra.clear();
        m_spectra.append(new ComponentSpectrum(QVector3D( 1, 0.6f, 0.4f), "Red"));
        m_spectra.append(new ComponentSpectrum(QVector3D( 1, 0.9f, 0.45f), "Yellow"));
        m_spectra.append(new ComponentSpectrum(QVector3D( 0.4f, 0.6f, 1.0f), "Blue"));
        m_spectra.append(new ComponentSpectrum(QVector3D( 1.0f, 1.0f, 1.0f), "White"));
        m_spectra.append(new ComponentSpectrum(QVector3D( 0.3f, 0.7f, 1.0f), "Cyan"));
        m_spectra.append(new ComponentSpectrum(QVector3D( 1.0f, 0.3f, 0.8f), "Purple"));

    }
    QStringList listSpectra() {
        QStringList ql;
        for (ComponentSpectrum* c : m_spectra)
            ql << c->name();
        return ql;
    }

    ComponentSpectrum* FindSpectrum(QString n) {
        for (ComponentSpectrum* c : m_spectra)
            if (c->name().toLower() == n.toLower())
                return c;
        return nullptr;
    }

    friend QDataStream& operator << ( QDataStream & s, Spectra& cs) {
        QVector<ComponentSpectrum> cplist;
        for (ComponentSpectrum* cp: cs.m_spectra)
            cplist.append(*cp);

        s << cplist;
        return s;
    }
    friend QDataStream& operator >> ( QDataStream & s, Spectra& cs) {
        QVector<ComponentSpectrum> cp;
        s >> cp;
        // must convert to pointer %)$#($
        cs.m_spectra.clear();
        for (ComponentSpectrum c: cp) {
            ComponentSpectrum* newc = new ComponentSpectrum();
            *newc = c;
            cs.m_spectra.append(newc);
        }
        return s;
    }



};




