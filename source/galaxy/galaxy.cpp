#include "source/galaxy/galaxyparams.h"
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponents.h"
#include <QFile>
#include <QDataStream>



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
        ngc = (GalaxyComponent*)new GalaxyComponentDisk();

    if (className=="stars")
        ngc = (GalaxyComponent*)new GalaxyComponentStars();

    if (ngc==nullptr) {
        qDebug() << "Error: could not instantiate :" << className;
        exit(1);
    }
    return ngc;
}

QVector<GalaxyComponent *> Galaxy::components() const
{
    return m_components;
}

GalaxyParams Galaxy::galaxyParams() const
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

void Galaxy::SetupComponents() {

    m_components.clear();
   // qDebug() << m_componentParams.size();
    for (ComponentParams* cp : m_componentParams) {
        qDebug() << "Setting up " << cp->className();
        GalaxyComponent* ngc = instantiate(cp->className());
        //GalaxyComponent* ngc = (GalaxyComponent)System.Activator.CreateInstance (System.Type.GetType (Settings.classDictionary[cp.className]));
        ngc->Initialize(cp, &m_galaxyParams);
        m_components.append (ngc);
    }
    //SetupSpectra();
}
/*	public void SetupSpectra() {
    foreach (GalaxyComponent gc in components) {
        gc.spectrum = Spectra.FindSpectrum(gc.componentParams.spectrum);
        if (gc.spectrum == null) {
            Debug.Log ("ERROR Could not find spectrum : " + gc.componentParams.spectrum);
        }
    }
}
*/
ComponentParams* Galaxy::AddComponent() {
    ComponentParams* cp = new ComponentParams();
    cp->setClassName("bulge");
    cp->setStrength(1);
    cp->setR0(1);
    m_componentParams.append(cp);
    SetupComponents();
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
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Could not open " << filename;
        return;
    }
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_6);

    out << *this;

    file.flush();
    file.close();

}


/*	public static void Save(string filename, Galaxy g) {
    XmlSerializer serializer = new XmlSerializer(typeof(Galaxy));
    TextWriter textWriter = new StreamWriter(filename);
    serializer.Serialize(textWriter, g);
    textWriter.Close();

}
*/
bool Galaxy::Load(QString filename) {
    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open " << filename;
        return false;
    }
    QDataStream in(&file);

    in >> *this;

    file.close();
    SetupComponents();
    return true;
}
