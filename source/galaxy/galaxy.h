#pragma once

#include "math.h"
#include <QString>
#include <QMatrix4x4>
#include <QVector>
#include "source/galaxy/galaxyparams.h"
#include "source/galaxy/componentparams.h"

class GalaxyComponent;

class Galaxy {
private:

    GalaxyParams m_galaxyParams;
    QString m_displayName = "";
    QMatrix4x4 m_rotation;
    QVector<ComponentParams*> m_componentParams;
    QVector<GalaxyComponent*> m_components;

public:

    void SetupComponents();
    ComponentParams* AddComponent(int);

    Galaxy() {}

    Galaxy(GalaxyParams* param, QString displayName, QVector<ComponentParams*> componentParams);
    GalaxyComponent* instantiate(QString className);

    Galaxy* Clone();
    void Save(QString filename);
    bool Load(QString filename);
    QVector<ComponentParams *>& componentParams();
    QVector<GalaxyComponent *> components() const;
    GalaxyParams& galaxyParams();

    friend QDataStream& operator << ( QDataStream & s, Galaxy& g) {
        s << g.displayName();
        s << g.galaxyParams();
        QVector<ComponentParams> cplist;
        for (ComponentParams* cp: g.componentParams())
            cplist.append(*cp);

        s << cplist;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, Galaxy& g) {
        QString dn;
        s >> dn;
        g.setDisplayName(dn);
        GalaxyParams gp;
        s >> gp;
        g.setGalaxyParams(gp);
        QVector<ComponentParams> cp;
        s >> cp;
        // must convert to pointer %)$#($
        g.componentParams().clear();
        for (ComponentParams c: cp) {
            ComponentParams* newc = new ComponentParams();
            *newc = c;
            g.componentParams().append(newc);
        }

        return s;
    }

    QString displayName() const;
    void setDisplayName(const QString &displayName);
    void setComponentParams(const QVector<ComponentParams* > &componentParams);
    void setGalaxyParams(const GalaxyParams &galaxyParams);
    void SetupSpectra();
};


