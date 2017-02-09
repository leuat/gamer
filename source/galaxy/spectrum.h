#pragma once

#include <QString>
#include <QVector>
#include <QVector3D>

class ComponentSpectrum {
private:
    QString m_name;
    QVector3D m_spectrum = QVector3D(0,0,0);
public:
    ComponentSpectrum(QVector3D s, QString n) {
        m_name = n;
        m_spectrum = s;
    }
    QString name() const;
    void setName(const QString &name);
    QVector3D spectrum() const;
    void setSpectrum(const QVector3D &spectrum);
};

class Spectra  {

    static QVector<ComponentSpectrum*> m_spectra;

public:
    static void PopulateSpectra() {

        m_spectra.append( new ComponentSpectrum(QVector3D( 1, 0.8f, 0.7f), "Red"));
        m_spectra.append( new ComponentSpectrum(QVector3D( 1, 0.9f, 0.55f), "Yellow"));
        m_spectra.append( new ComponentSpectrum(QVector3D( 0.6f, 0.7f, 1.0f), "Blue"));
        m_spectra.append( new ComponentSpectrum(QVector3D( 1.0f, 1.0f, 1.0f), "White"));
        m_spectra.append( new ComponentSpectrum(QVector3D( 0.3f, 0.7f, 1.0f), "Cyan"));
        m_spectra.append( new ComponentSpectrum(QVector3D( 1.0f, 0.4f, 0.8f), "Purple"));

    }
    static QStringList listSpectra() {
        QStringList ql;
        for (ComponentSpectrum* c : m_spectra)
            ql << c->name();
        return ql;
    }

    static ComponentSpectrum* FindSpectrum(QString n) {
        for (ComponentSpectrum* c : m_spectra)
            if (c->name() == n)
                return c;
        return nullptr;
    }

};




