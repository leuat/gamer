#include "source/galaxy/galaxyparams.h"
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponents.h"
#include <QFile>
#include <QDataStream>
#include "source/util/gmessages.h"
#include "source/util/util.h"


QVector<ComponentParams* >& Galaxy::componentParams()
{
    return m_componentParams;
}

GalaxyComponent* Galaxy::instantiate(QString className) {
    GalaxyComponent* ngc = nullptr;
    if (className=="bulge")
        ngc = (GalaxyComponent*)new GalaxyComponentBulge();

    if (className=="disk")
        ngc = (GalaxyComponent*)new GalaxyComponentDisk();

    if (className=="dust")
        ngc = (GalaxyComponent*)new GalaxyComponentDust();

    if (className=="dust2")
        ngc = (GalaxyComponent*)new GalaxyComponentDust2();

    if (className=="stars")
        ngc = (GalaxyComponent*)new GalaxyComponentStars();

    if (ngc==nullptr) {
        qDebug() << "Error: could not instantiate :" << className;
        exit(1);
    }
    return ngc;
}

QVector<GalaxyComponent*>& Galaxy::components()
{
    return m_components;
}

GalaxyParams& Galaxy::galaxyParams()
{
    return m_galaxyParams;
}

QString Galaxy::displayName() const
{
    return m_displayName;
}

void Galaxy::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
}

void Galaxy::setComponentParams(const QVector<ComponentParams* > &componentParams)
{
    m_componentParams = componentParams;
}


void Galaxy::setGalaxyParams(const GalaxyParams &galaxyParams)
{
    m_galaxyParams = galaxyParams;
}

void Galaxy::SetupSpectra(RenderingParams* rp)
{
//    GMessages::Debug("Setting up components");
    for (GalaxyComponent* gc : m_components) {
        gc->setSpectrum(rp->spectra().FindSpectrum(gc->getComponentParams().spectrum()));
        if (gc->getSpectrum() == nullptr) {
            gc->setSpectrum(rp->spectra().m_default);
            GMessages::Message("Warning: Could not find spectrum '" + gc->getComponentParams().spectrum() + "', using white as default.");
        }
    }
}

QString Galaxy::VerifySpectra(RenderingParams *rp)
{
    for (GalaxyComponent* gc : m_components) {
        if (rp->spectra().FindSpectrum(gc->getComponentParams().spectrum())==nullptr)
            return gc->getComponentParams().spectrum();
    }
    return "";

}

void Galaxy::SetupComponents(RenderingParams* rp) {

    m_components.clear();
    for (ComponentParams* cp : m_componentParams) {
        GalaxyComponent* ngc = instantiate(cp->className());
        ngc->Initialize(cp, &m_galaxyParams);
        m_components.append (ngc);
    }
    SetupSpectra(rp);
}

ComponentParams* Galaxy::AddComponent(int count) {
    ComponentParams* cp = new ComponentParams();
    cp->setClassName("bulge");
    cp->setStrength(30);
    cp->setR0(5);
    cp->setSpectrum("Yellow");
    m_componentParams.append(cp);

    if (count>1) {
        cp = new ComponentParams();
        cp->setClassName("disk");
        cp->setStrength(900);
        cp->setR0(0.4);
        cp->setArm(0.3);
        cp->setNoiseTilt(0.3);
        cp->setSpectrum("Blue");
        cp->setScale(1);
        //    cp->setN
        m_componentParams.append(cp);
    }

    if (count>2) {
        cp = new ComponentParams();
        cp->setClassName("dust");
        cp->setStrength(250);
        cp->setR0(0.45);
        cp->setArm(0.25);
        cp->setZ0(0.015);
        cp->setNoiseTilt(1);
        cp->setSpectrum("Blue");
        cp->setScale(1);
        //    cp->setN
        m_componentParams.append(cp);
    }

//    SetupComponents(rp);
    return cp;

}

Galaxy::Galaxy(GalaxyParams* param, QString displayName, QVector<ComponentParams*> componentParams)  {
    m_galaxyParams = *param;
    m_displayName = displayName;
    m_componentParams = componentParams;
}

Galaxy* Galaxy::Clone() {
    Galaxy* g = new Galaxy(&m_galaxyParams, m_displayName, m_componentParams);
    for (GalaxyComponent* gc : m_components) {
        GalaxyComponent* ngc = instantiate(gc->getComponentParams().className());
        ComponentParams cp = gc->getComponentParams();
        ngc->Initialize(&cp, &m_galaxyParams);
        g->components().append (ngc);
    }
    return g;
}

void Galaxy::Save(QString filename)
{
    QFile file(Util::path + filename);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open " << filename;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_6);

    out << *this;

    file.flush();
    file.close();
    GMessages::Debug("Saved galaxy to '" +filename +"'.");
}


bool Galaxy::Load(QString filename) {
    QFile file(Util::path + filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open " << filename;
        return false;
    }
    QDataStream in(&file);

    in >> *this;

    file.close();
//    SetupComponents(rp);
    GMessages::Debug("Loaded galaxy '" +filename +"'.");

    return true;
}
