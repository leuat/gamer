#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QStringList>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QDirIterator>
#include "source/galaxy/spectrum.h"
#include "source/util/gmessages.h"
#include "source/util/util.h"
#include "dialogrendererhelp.h"
#include "dialogabout.h"
#include "source/util/fitsio.h"
#include <QElapsedTimer>
#include "source/util/random.h"
#include "source/galaxy/rasterthread.h"
//#include <QListData>



float MainWindow::m_version = 1.02;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    Util::path = QCoreApplication::applicationDirPath() + "/../../";
#if defined(Q_OS_WIN)
    Util::path = QCoreApplication::applicationDirPath() +"/";

#endif
/*    m_rastGalaxy = new RasterThread(&m_renderingParams);
    m_rastScene = new RasterThread(&m_renderingParams);
*/

    m_rastGalaxy = new Rasterizer(&m_renderingParams);
    m_rastScene = new Rasterizer(&m_renderingParams);

    ui->myGLWidget->setRenderingParams(&m_renderingParams);
    GMessages::Initialize(ui->lstMessages);

    PopulateImageSize();
    m_renderingParams.Load(m_RenderParamsFilename);
//    qDebug() << m_renderingParams.spectra().m_spectra.size();

    if (m_renderingParams.spectra().m_spectra.size()==0)
        m_renderingParams.spectra().PopulateSpectra();

    m_curComponentSpectrum = m_renderingParams.spectra().m_spectra[0];
    UpdateSpectrumParamsGUI();
    UpdateRenderingParamsGUI();

    // Try to load
//    m_renderingParams.setCurrentGalaxy("Spiral");
    m_galaxy.Load(m_renderingParams.galaxyDirectory() + m_renderingParams.currentGalaxy() + ".gax");

    if (m_galaxy.componentParams().size()==0) {
        m_galaxy.AddComponent(10);
        m_renderingParams.setCurrentGalaxy(m_galaxy.galaxyParams().name());
    }
    m_rasterizer = m_rastGalaxy;
    PrepareNewGalaxy();

    // Adding a single galaxy to the rasterizer
    m_rastGalaxy->AddGalaxy(new GalaxyInstance(&m_galaxy, m_galaxy.galaxyParams().name(),
                                              QVector3D(0.0,0.0,0), QVector3D(0,1,0).normalized(), 1, 0)  );

    // Copy rasterizer to queue renderer
    //m_renderQueue.setRasterizer(m_rasterizer);



    UpdateSceneGUI();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start(25);
    ui->myGLWidget->setRedraw(true);
    setWindowTitle("Gamer " + QString::number(m_version));


//    ui->btnSkybox->setVisible(false);

//    m_renderingParams.camera().setCamera(QVector3D(0.1,0,0));
//    m_renderingParams.camera().setUp(QVector3D(0,1,0));
      m_renderingParams.camera().setTarget(QVector3D(0,0,0));


//    ui->leRayStep->setVisible(false);
//    ui->lblRaystep->setVisible(false);


}

MainWindow::~MainWindow()
{
    m_rasterizer->Abort();
    delete m_rasterizer;
    delete ui;
}

void MainWindow::loadFile()
{
}


void MainWindow::on_renderButton_clicked()
{
    m_state = State::Rendering;
    Render(false);
}

void MainWindow::on_actionLoad_triggered()
{
/*    if (!m_galaxy.Load("test.gax")) {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.exec();
        return;
    }
    PrepareNewGalaxy();
    RenderPreview(m_renderingParams.previewSize());*/
}

void MainWindow::on_actionSave_triggered()
{
    //m_galaxy.Save("test.gax");
}

void MainWindow::PopulateCmbComponentTypes() {
    //ui->cmbComponentType->clear();
    if (m_curComponentParams != nullptr) {
        if (ui->cmbComponentType->count()==0)
          ui->cmbComponentType->addItems(QStringList() << "bulge" << "disk" << "dust" << "stars" << "dust2");
        //int idx = ui->cmbComponentType->findData(m_curComponentParams->className());
        ui->cmbComponentType->setCurrentText(m_curComponentParams->className());
    }

}

void MainWindow::PopulateCmbSpectra()
{
    ui->cmbSpectrum->clear();
    ui->cmbSpectrum->addItems(m_renderingParams.spectra().listSpectra());
    if (m_curComponentParams!=nullptr)
        ui->cmbSpectrum->setCurrentText(m_curComponentParams->spectrum());
}

void MainWindow::PopulateImageSize()
{
    ui->cmbImageSize->clear();
    ui->cmbPreviewSize->clear();
    QStringList l = QStringList() << "16" <<"32" << "48" << "64" << "80" << "96" << "128"
                    << "256" << "384" <<  "512" << "768" << "1024"
                    << "1200" << "1600" << "2048";
    ui->cmbImageSize->addItems(l);
    l = QStringList() << "16" <<"32" << "48" << "64" << "80" << "96";

    ui->cmbPreviewSize->addItems(l);

    l = QStringList() << "OpenMP" <<"Threaded";

    ui->cmbRenderer->addItems(l);

}

void MainWindow::PopulateGalaxyList()
{


//    qDebug() << Util::path;
    QDirIterator it(Util::path + m_renderingParams.galaxyDirectory(),
                    QStringList() << "*.gax", QDir::Files, QDirIterator::Subdirectories);
    ui->lstGalaxies->clear();
    ui->lstSceneGalaxies->clear();
    while (it.hasNext()) {
        QString filename = it.next();
        QString name = filename.split("/").last();
        ui->lstGalaxies->addItem(name);
        ui->lstSceneGalaxies->addItem(name);
    }
}

void MainWindow::PopulateSpectraEditList()
{
    ui->cmbSpectraEdit->clear();
    ui->cmbSpectraEdit->addItems(m_renderingParams.spectra().listSpectra());
    if (m_curComponentSpectrum!=nullptr)
        ui->cmbSpectraEdit->setCurrentText(m_curComponentSpectrum->name());
    PopulateCmbSpectra();
}

void MainWindow::UpdateComponentsGUIbyType()
{
    if (m_curComponentParams->className()=="bulge") {
        ui->leStrength->setVisible(true);
        ui->leArm->setVisible(false);
        ui->leZ0->setVisible(false);
        ui->leR0->setVisible(true);
        ui->leInner->setVisible(false);
        ui->leDelta->setVisible(false);
        ui->leWinding->setVisible(false);
        ui->leScale->setVisible(false);
        ui->leDelta->setVisible(false);
        ui->leNoiseOffset->setVisible(false);
        ui->leNoiseTilt->setVisible(false);
        ui->lePersistence->setVisible(false);
    }
    else {
        ui->leStrength->setVisible(true);
        ui->leArm->setVisible(true);
        ui->leZ0->setVisible(true);
        ui->leR0->setVisible(true);
        ui->leDelta->setVisible(true);
        ui->leInner->setVisible(true);
        ui->leWinding->setVisible(true);
        ui->leScale->setVisible(true);
        ui->leDelta->setVisible(true);
        ui->leNoiseOffset->setVisible(true);
        ui->leNoiseTilt->setVisible(true);
        ui->lePersistence->setVisible(true);
    }
}

void MainWindow::UpdateGalaxyGUI()
{
    ui->leGalaxyName->setText(m_galaxy.galaxyParams().name());
    ui->leWindingB->setText(QString::number(m_galaxy.galaxyParams().windingB()));
    ui->leWindingN->setText(QString::number(m_galaxy.galaxyParams().windingN()));
    ui->leNoArms->setText(QString::number(m_galaxy.galaxyParams().noArms()));
    ui->leArm1->setText(QString::number(m_galaxy.galaxyParams().arm1()));
    ui->leArm2->setText(QString::number(m_galaxy.galaxyParams().arm2()));
    ui->leArm3->setText(QString::number(m_galaxy.galaxyParams().arm3()));
    ui->leArm4->setText(QString::number(m_galaxy.galaxyParams().arm4()));
    //ui->leBulgeDust->setText(QString::number(m_galaxy.galaxyParams().bulgeDust()));
}

void MainWindow::UpdateGalaxyData()
{
    m_galaxy.galaxyParams().setName(ui->leGalaxyName->text());
    m_galaxy.galaxyParams().setWindingB(ui->leWindingB->text().toFloat());
    m_galaxy.galaxyParams().setWindingN(ui->leWindingN->text().toFloat());
    m_galaxy.galaxyParams().setNoArms(ui->leNoArms->text().toFloat());
    m_galaxy.galaxyParams().setArm1(ui->leArm1->text().toFloat());
    m_galaxy.galaxyParams().setArm2(ui->leArm2->text().toFloat());
    m_galaxy.galaxyParams().setArm3(ui->leArm3->text().toFloat());
    m_galaxy.galaxyParams().setArm4(ui->leArm4->text().toFloat());
   // m_galaxy.galaxyParams().setBulgeDust(ui->leBulgeDust->text().toFloat());

    RenderPreview(m_renderingParams.previewSize());
    SaveGalaxy();
}

void MainWindow::UpdateComponentsData() {

    m_curComponentParams->setStrength( ui->leStrength->text().toFloat());
    m_curComponentParams->setArm( ui->leArm->text().toFloat());
    m_curComponentParams->setZ0( ui->leZ0->text().toFloat());
    m_curComponentParams->setR0( ui->leR0->text().toFloat());
    m_curComponentParams->setInner( ui->leInner->text().toFloat());
    m_curComponentParams->setDelta(ui->leDelta->text().toFloat());
    m_curComponentParams->setWinding( ui->leWinding->text().toFloat());
    m_curComponentParams->setScale( ui->leScale->text().toFloat());
    m_curComponentParams->setNoiseOffset( ui->leNoiseOffset->text().toFloat());
    m_curComponentParams->setNoiseTilt( ui->leNoiseTilt->text().toFloat());
    m_curComponentParams->setKs(ui->lePersistence->text().toFloat());
    m_curComponentParams->setActive(ui->chkIsActive->isChecked()==true);
    m_curComponentParams->setSpectrum(ui->cmbSpectrum->currentText());
    m_curComponentParams->setName(ui->leComponentName->text());
    SaveGalaxy();
    RenderPreview(m_renderingParams.previewSize());

}

void MainWindow::UpdateComponentsGUI()
{
    PopulateCmbComponentTypes();
    PopulateCmbSpectra();
    UpdateComponentsGUIbyType();
    ui->leStrength->setText(QString::number(m_curComponentParams->strength()));
    ui->leArm->setText(QString::number(m_curComponentParams->arm()));
    ui->leZ0->setText(QString::number(m_curComponentParams->z0()));
    ui->leR0->setText(QString::number(m_curComponentParams->r0()));
    ui->leInner->setText(QString::number(m_curComponentParams->inner()));
    ui->leDelta->setText(QString::number(m_curComponentParams->delta()));
    ui->leWinding->setText(QString::number(m_curComponentParams->winding()));
    ui->leScale->setText(QString::number(m_curComponentParams->scale()));
    ui->leNoiseOffset->setText(QString::number(m_curComponentParams->noiseOffset()));
    ui->lePersistence->setText(QString::number(m_curComponentParams->ks()));
    ui->leNoiseTilt->setText(QString::number(m_curComponentParams->noiseTilt()));
    ui->chkIsActive->setChecked(m_curComponentParams->active()==1);
    ui->leComponentName->setText(m_curComponentParams->name());
}

void MainWindow::PopulateCmbComponents() {
//    int idx = ui->cmbComponents->currentIndex();
    ui->cmbComponents->clear();
//    int idx = 0
    for (ComponentParams* gc: m_galaxy.componentParams()) {
        QString s = gc->name();//gc->className() + " " + QString::number(i);
        ui->cmbComponents->addItem(s);
        if (m_curComponentParams == gc)
            ui->cmbComponents->setCurrentText(gc->name());
    }
//    if (idx!=-1)
//        ui->cmbComponents->setCurrentIndex(idx);


}

void MainWindow::UpdateGUI()
{
    PopulateCmbComponents();
    UpdateGalaxyGUI();
}

void MainWindow::UpdateRenderingParamsGUI()
{
//    qDebug() << m_renderingParams.size();
    ui->cmbImageSize->setCurrentText(QString::number(m_renderingParams.size()));
    ui->cmbPreviewSize->setCurrentText(QString::number(m_renderingParams.previewSize()));
    ui->leRayStep->setText( QString::number(m_renderingParams.rayStep()));
    ui->leGalaxyDir->setText(m_renderingParams.galaxyDirectory());
    ui->leSceneDir->setText(m_renderingParams.sceneDirectory());
    ui->leImageDir->setText(m_renderingParams.imageDirectory());
    ui->leFOV->setText(QString::number(m_renderingParams.camera().perspective()));
    ui->hsExposure->setValue(m_renderingParams.exposure()*m_postSliderScale);
    ui->hsGamma->setValue(m_renderingParams.gamma()*m_postSliderScale);
    ui->hsSaturation->setValue(m_renderingParams.saturation()*m_postSliderScale);
    PopulateGalaxyList();
    PopulateSpectraEditList();
    UpdateStarsGUI();
}




void MainWindow::UpdateRenderingParamsData()
{
    m_renderingParams.setSize(ui->cmbImageSize->currentText().toInt());
    m_renderingParams.setPreviewSize(ui->cmbPreviewSize->currentText().toInt());
    m_renderingParams.setRayStep(ui->leRayStep->text().toFloat());
    m_rasterizer->setNewSize(m_renderingParams.size());
    m_renderingParams.setGalaxyDirectory(ui->leGalaxyDir->text());
    m_renderingParams.setSceneDirectory(ui->leSceneDir->text());
    m_renderingParams.setImageDirectory(ui->leImageDir->text());
    m_renderingParams.camera().setPerspective(ui->leFOV->text().toFloat());


    m_renderingParams.Save(m_RenderParamsFilename);
    RenderPreview(m_renderingParams.previewSize());
}

void MainWindow::UpdatePostProcessingData()
{
    m_renderingParams.setExposure(ui->hsExposure->value()/m_postSliderScale);
    m_renderingParams.setGamma(ui->hsGamma->value()/m_postSliderScale);
    m_renderingParams.setSaturation(ui->hsSaturation->value()/m_postSliderScale);
//    m_rasterizer->RenderDirect();
    m_renderingParams.Save(m_RenderParamsFilename);
    UpdateImage();

}

void MainWindow::UpdateSpectrumParamsGUI()
{
    if (m_curComponentSpectrum==nullptr)
        return;

    ui->leSpectrumName->setText( m_curComponentSpectrum->m_name );
    ui->leSpectrumRed->setText(QString::number(m_curComponentSpectrum->m_spectrum.x()));
    ui->leSpectrumGreen->setText(QString::number(m_curComponentSpectrum->m_spectrum.y()));
    ui->leSpectrumBlue->setText(QString::number(m_curComponentSpectrum->m_spectrum.z()));

}

void MainWindow::UpdateSpectrumParamsData()
{
    if (m_curComponentSpectrum==nullptr)
        return;

    m_curComponentSpectrum->m_name = ui->leSpectrumName->text();
    m_curComponentSpectrum->m_spectrum.setX(ui->leSpectrumRed->text().toFloat());
    m_curComponentSpectrum->m_spectrum.setY(ui->leSpectrumGreen->text().toFloat());
    m_curComponentSpectrum->m_spectrum.setZ(ui->leSpectrumBlue->text().toFloat());
    PopulateSpectraEditList();
    PopulateCmbSpectra();

    m_renderingParams.Save(m_RenderParamsFilename);
    RenderPreview(m_renderingParams.previewSize());
}

void MainWindow::UpdateStarsGUI()
{
    ui->leNoStars->setText( QString::number(m_renderingParams.noStars()));
    ui->leBaseSize->setText( QString::number(m_renderingParams.starSize()));
    ui->leSpreadSize->setText( QString::number(m_renderingParams.starSizeSpread()));
    ui->leStarStrength->setText( QString::number(m_renderingParams.starStrength()));
}

void MainWindow::UpdateStarsData()
{
    m_renderingParams.setNoStars(ui->leNoStars->text().toInt());
    m_renderingParams.setStarSize(ui->leBaseSize->text().toFloat());
    m_renderingParams.setStarSizeSpread(ui->leSpreadSize->text().toFloat());
    m_renderingParams.setStarStrength(ui->leStarStrength->text().toFloat());
    m_rasterizer->RenderStars();
    UpdateImage();
}


void MainWindow::Render(bool queue)
{

    QString problemSpectrum = m_galaxy.VerifySpectra(&m_renderingParams);
    if (problemSpectrum!="") {
        QMessageBox msgBox;
        msgBox.setText("The spectrum named '" + problemSpectrum +"' is missing. Please set up this spectrum in the 'spectra' tab before proceeding. Rendering halted.");
        msgBox.exec();
        return;
    }


    EnableGUIEditing(false);
    m_renderingParams.Save(m_RenderParamsFilename);
    m_rasterizer->setNewSize(m_renderingParams.size());

    if (queue) {
        QString filename = Util::getFileName(Util::path + m_renderingParams.imageDirectory(),"QueuedImage","png");
        m_renderQueue.Add(m_rasterizer, m_renderingParams,filename);
    }
    else
        m_rasterizer->Render();

}

void MainWindow::RenderDirect()
{
    m_rasterizer->RenderDirect();
    UpdateImage();

}

void MainWindow::RenderPreview(int size)
{
    int oldSize = m_renderingParams.size();
    float oldStep = m_renderingParams.rayStep();
    m_renderingParams.setRayStep(0.025);
    m_rasterizer->setNewSize(size);
    RenderDirect();
    m_renderingParams.setSize(oldSize);
    m_renderingParams.setRayStep(oldStep);
}


void MainWindow::RenderSkybox() {

    QVector<QVector3D> planes, ups;
    planes.resize(6);
    ups.resize(6);
    planes[0] = QVector3D(0,0,-1);
    planes[1] = QVector3D(0,0,1);
    planes[2] = QVector3D(0,1,0);
    planes[3] = QVector3D(0,-1,0);
    planes[4] = QVector3D(1,0,0);
    planes[5] = QVector3D(-1,0,0);

    ups[0] = QVector3D(0,-1,0);
    ups[1] = QVector3D(0,-1,0);
    ups[2] = QVector3D(0,0,1);
    ups[3] = QVector3D(0,0,-1);
    ups[4] = QVector3D(0,-1,0);
    ups[5] = QVector3D(0,-1,0);

    QVector<QString> names;
    names.resize(6);
    names[0] ="Z-";
    names[1] ="Z+";
    names[2] ="Y+";
    names[3] ="Y-";
    names[4] ="X+";
    names[5] ="X-";

    //m_renderingParams.camera().setRotMatrix();
    RenderingParams reset = m_renderingParams;
    //RP.camera.setRotMatrix(resetCamera.GetRotationMatrix());

    for (int i=0;i<6;i++) {
        m_renderingParams.camera().setRotMatrix(reset.camera().GetRotationMatrix()  );
        m_renderingParams.camera().setCamera( reset.camera().camera() );
        m_renderingParams.camera().setTarget( reset.camera().camera() + planes[i] );
        m_renderingParams.camera().setUp(ups[i]);
        m_renderingParams.camera().setPerspective(90);
        //QString fname = Util::getFileName(m_renderingParams.imageDirectory(),"SkyboxZ-","png");
        QString fname = "Skybox" + names[i];
        m_renderQueue.Add(m_rasterizer, m_renderingParams, fname);
    }

    m_renderingParams = reset;

}


void MainWindow::EnableGUIEditing(bool value)
{
    //if (m_state == State::Rendering)
    ui->myGLWidget->disableInput(!value);

    ui->btnSceneMode->setEnabled(value);
    ui->cmbRenderer->setEnabled(value);
    ui->leComponentName->setEnabled(value);
    ui->actionLoad->setEnabled(value);
    ui->actionSave->setEnabled(value);
    ui->renderButton->setEnabled(value);
    ui->btnNewComponent->setEnabled(value);
    ui->btnNewComponent_2->setEnabled(value);
    ui->leArm->setEnabled(value);
    ui->leArm1->setEnabled(value);
    ui->leArm2->setEnabled(value);
    ui->leArm3->setEnabled(value);
    ui->leArm4->setEnabled(value);
    ui->leDelta->setEnabled(value);
    ui->btnSkybox->setEnabled(value);
    ui->leGalaxyName->setEnabled(value);
    ui->leNoArms->setEnabled(value);
    ui->leNoiseOffset->setEnabled(value);
    ui->leNoiseTilt->setEnabled(value);
    ui->lePersistence->setEnabled(value);
    ui->leR0->setEnabled(value);
    ui->leRayStep->setEnabled(value);
    ui->leScale->setEnabled(value);
    ui->leStrength->setEnabled(value);
    ui->leWinding->setEnabled(value);
    ui->leWindingB->setEnabled(value);
    ui->leWindingN->setEnabled(value);
    ui->leZ0->setEnabled(value);
    ui->leFOV->setEnabled(value);
    ui->leGalaxyDir->setEnabled(value);
    ui->leSceneDir->setEnabled(value);
    ui->leImageDir->setEnabled(value);
    ui->cmbComponents->setEnabled(value);
    ui->cmbComponentType->setEnabled(value);
    ui->cmbImageSize->setEnabled(value);
    ui->cmbPreviewSize->setEnabled(value);

    ui->cmbSpectrum->setEnabled(value);
    ui->chkIsActive->setEnabled(value);

    ui->btnNew->setEnabled(value);
    ui->btnClone->setEnabled(value);
    ui->btnDelete->setEnabled(value);
    ui->lstGalaxies->setEnabled(value);

    ui->cmbSpectraEdit->setEnabled(value);
    ui->leSpectrumRed->setEnabled(value);
    ui->leSpectrumGreen->setEnabled(value);
    ui->leSpectrumBlue->setEnabled(value);
    ui->leSpectrumName->setEnabled(value);

    ui->btnNewSpectrum->setEnabled(value);
    ui->btnDeleteSpectrum->setEnabled(value);

    ui->btnQueue->setEnabled(value);
}

void MainWindow::SaveGalaxy()
{
    QString filename = m_renderingParams.galaxyDirectory() + m_galaxy.galaxyParams().name() + ".gax";
    m_galaxy.Save(filename);
}

void MainWindow::CreateNewGalaxy()
{
    m_galaxy = Galaxy();
    m_galaxy.AddComponent(10);
    PrepareNewGalaxy();
    m_galaxy.galaxyParams().setName("NewGalaxy");
    SaveGalaxy();
    PopulateGalaxyList();
    RenderPreview(m_renderingParams.previewSize());
    UpdateComponentsGUI();
    UpdateGalaxyGUI();
}


void MainWindow::loop()
{
    // Important: call RenderQueue first!
    m_renderQueue.Update();
    // Preview image
    if (ui->myGLWidget->redraw()) {
        RenderPreview(m_renderingParams.previewSize());
        m_renderingParams.Save(m_RenderParamsFilename);
    }


    Rasterizer* curRast = m_rasterizer;
    if (m_state==State::Rendering) {
        EnableGUIEditing(false);
        if (curRast->getState()==Rasterizer::State::done) {
            //ui->myGLWidget->SetTexture(curRast->getBuffer());
            UpdateImage();
        }
        if (curRast->getState()==Rasterizer::State::rendering) {
            UpdateImage();
        }
    }
//    UpdateImage();


    if (m_renderQueue.isRendering()) {
        m_state = State::Queueing;
        EnableGUIEditing(true);
        ui->renderButton->setEnabled(false);
        curRast = &m_renderQueue.current()->rasterizer();

    }
    else {
 //       m_state = State::Idle;
//        EnableGUIEditing(true);
        curRast = m_rasterizer;
    }

 /*   if (m_state==State::Queueing) {
        if (m_renderQueue.current()==nullptr) {
            setEnabled(true);
        }
        else
            curRast = &m_renderQueue.current()->rasterizer();
    }*/
    ui->pgbRendering->setValue((int)(curRast->getPercentDone()*100.0)+1);
    if (curRast->getState()==Rasterizer::State::done) {
        EnableGUIEditing(true);
        m_state = State::Idle;
        curRast->setState(Rasterizer::State::idle);
        UpdateImage();

    }

    // ETA time
    if (curRast->getState()==Rasterizer::State::rendering) {
        float time = curRast->getTimer().elapsed();
        float percentage = curRast->getPercentDone();
        float timeLeft = time/(percentage) - time;
        ui->lblEta->setText("ETA: " + Util::MilisecondToString(timeLeft));
    }
    else
        ui->lblEta->setText("");
}

void MainWindow::on_btnNewComponent_clicked()
{
    m_curComponentParams = m_galaxy.AddComponent(1);
    UpdateGUI();
    UpdateComponentsGUI();
}

void MainWindow::on_cmbComponentType_activated(const QString &arg1)
{
    if (m_curComponentParams==nullptr)
        return;
//    qDebug() << arg1;
    m_curComponentParams->setClassName(arg1);
//    ui->cmbComponentType->setCurrentText(arg1);
    m_galaxy.SetupComponents(&m_renderingParams);
    UpdateGUI();
    UpdateComponentsGUI();

}

void MainWindow::on_cmbComponents_activated(int index)
{
    m_curComponentParams = (m_galaxy.componentParams()[index]);
    UpdateComponentsGUI();

}


void MainWindow::on_leStrength_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_lePersistence_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leNoiseTilt_editingFinished()
{
    UpdateComponentsData();
}

void MainWindow::on_leNoiseOffset_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leScale_editingFinished()
{
    UpdateComponentsData();
}

void MainWindow::on_leWinding_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leDelta_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leR0_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leZ0_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leArm_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_chkIsActive_clicked()
{
    UpdateComponentsData();

}

void MainWindow::on_cmbSpectrum_activated(const QString &arg1)
{
    if (m_curComponentParams==nullptr)
        return;
    m_curComponentParams->setSpectrum(arg1);
    ui->cmbSpectrum->setCurrentText(arg1);
    m_galaxy.SetupComponents(&m_renderingParams);
    UpdateComponentsData();
}



void MainWindow::on_leGalaxyName_editingFinished()
{
    QFile file (Util::path +m_renderingParams.galaxyDirectory() + m_galaxy.galaxyParams().name() + ".gax");
    file.remove();
    UpdateGalaxyData();
    PopulateGalaxyList();
}

void MainWindow::on_leWindingB_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leWindingN_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leNoArms_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leArm1_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leArm2_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leArm3_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::on_leArm4_editingFinished()
{
    UpdateGalaxyData();
}

void MainWindow::PrepareNewGalaxy()
{
    if (m_galaxy.componentParams().size()!=0)
        m_curComponentParams = m_galaxy.componentParams()[0];
    UpdateGUI();
    UpdateComponentsGUI();

}

void MainWindow::UpdateImage()
{
    m_rasterizer->AssembleImage();
    ui->myGLWidget->SetTexture(m_rasterizer->getBuffer());

}

void MainWindow::on_actionRender_triggered()
{
    m_state = State::Rendering;
    Render(false);
}

void MainWindow::on_cmbImageSize_activated(const QString &arg1)
{
    UpdateRenderingParamsData();
}

void MainWindow::on_leRayStep_editingFinished()
{
    UpdateRenderingParamsData();
}

void MainWindow::on_btnAbort_clicked()
{
    m_rasterizer->Abort();
    m_renderQueue.Abort();
    m_state = State::Idle;
    EnableGUIEditing(true);

}

void MainWindow::on_cmbPreviewSize_activated(const QString &arg1)
{
    UpdateRenderingParamsData();

}

void MainWindow::on_leGalaxyDir_editingFinished()
{
    UpdateRenderingParamsData();
}


void MainWindow::on_lstGalaxies_itemDoubleClicked(QListWidgetItem *item)
{
    QString filename = m_renderingParams.galaxyDirectory() + item->text();
    m_galaxy.Load(filename);
    QString name = item->text().split(".")[0];
    m_galaxy.galaxyParams().setName(name);
    m_renderingParams.setCurrentGalaxy(name);
    //ui->lblCurrentGalaxy->setText(name);
    PrepareNewGalaxy();
    RenderPreview(m_renderingParams.previewSize());
    m_renderingParams.Save(m_RenderParamsFilename);
}

void MainWindow::on_btnNew_clicked()
{
    CreateNewGalaxy();
}

void MainWindow::on_leFOV_editingFinished()
{
    UpdateRenderingParamsData();
}

void MainWindow::on_hsExposure_sliderMoved(int position)
{
    UpdatePostProcessingData();
}

void MainWindow::on_hsGamma_sliderMoved(int position)
{
    UpdatePostProcessingData();
}

void MainWindow::on_hsSaturation_sliderMoved(int position)
{
    UpdatePostProcessingData();
}

void MainWindow::on_btnSaveImage_clicked()
{
    QString baseFile = Util::getFileName(Util::path + m_renderingParams.imageDirectory(),"Image","png");
    QString filename = m_renderingParams.imageDirectory() +
            baseFile +".png";
    if (ui->chkSaveFits->isChecked()) {
        FitsIO::SaveFloat(Util::path + m_renderingParams.imageDirectory() + baseFile + "_red.fits", 0, m_rasterizer->getRenderBuffer());
        FitsIO::SaveFloat(Util::path + m_renderingParams.imageDirectory() + baseFile + "_green.fits", 1, m_rasterizer->getRenderBuffer());
        FitsIO::SaveFloat(Util::path + m_renderingParams.imageDirectory() + baseFile + "_blue.fits", 2, m_rasterizer->getRenderBuffer());
    }
//      FitsIO::SaveFloat(m_renderingParams.imageDirectory() +"test.fits", 0, m_rasterizer->getRenderBuffer());

    m_rasterizer->getImageShadowBuffer()->save(Util::path + filename);
    GMessages::Message("Galaxy png saved to " + filename);

}

void MainWindow::on_leImageDir_editingFinished()
{
    UpdateRenderingParamsData();
}

void MainWindow::on_leSceneDir_editingFinished()
{
    UpdateRenderingParamsData();

}

void MainWindow::on_btnDelete_clicked()
{
    QMessageBox msgBox;
    QString gax = ui->lstGalaxies->currentItem()->text();
    msgBox.setText("Are you sure you wish to delete the galaxy '" + gax + "'?");
    msgBox.setInformativeText("This action cannot be undone.");
    msgBox.setStandardButtons(QMessageBox::Ok |  QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret==QMessageBox::Ok) {
        QString filename= m_renderingParams.galaxyDirectory() + gax;
        QFile file (Util::path +filename);
        file.remove();
        PopulateGalaxyList();
    }
}

void MainWindow::on_btnNewComponent_2_clicked()
{
    if (m_galaxy.componentParams().size()<=1)
        return;

    m_galaxy.componentParams().removeAll(m_curComponentParams);
    m_galaxy.SetupComponents(&m_renderingParams);
    m_curComponentParams = m_galaxy.componentParams()[0];
    UpdateComponentsGUI();
    PopulateCmbComponents();
}

void MainWindow::on_btnClone_clicked()
{
    QString name = m_galaxy.galaxyParams().name();
    m_galaxy.galaxyParams().setName(name+"Clone");
    m_renderingParams.setCurrentGalaxy(name + "Clone");
    SaveGalaxy();
    PopulateGalaxyList();
    UpdateGalaxyGUI();
}

void MainWindow::on_pushButton_clicked()
{
    DialogRendererHelp* dr = new DialogRendererHelp("Renderer panel", Util::loadTextFile(":/TabRenderingHelp.txt") );
    dr->show();
}

void MainWindow::on_btnHelpGalaxy_clicked()
{
    DialogRendererHelp* dr = new DialogRendererHelp("Galaxy parameters panel", Util::loadTextFile(":/TabGalaxyHelp.txt") );
    dr->show();

}

void MainWindow::on_btnHelpComponents_clicked()
{
    DialogRendererHelp* dr = new DialogRendererHelp("Components parameters panel", Util::loadTextFile(":/TabComponentsHelp.txt") );
    dr->show();

}

void MainWindow::on_btnHelpPostProcessing_clicked()
{
    DialogRendererHelp* dr = new DialogRendererHelp("Post-processing tools", Util::loadTextFile(":/TabPostProcessingHelp.txt") );
    dr->show();
}


void MainWindow::on_actionAbout_triggered()
{
    DialogAbout* da = new DialogAbout(m_version);
    da->show();
}

void MainWindow::on_cmbSpectraEdit_activated(const QString &arg1)
{
    m_curComponentSpectrum = m_renderingParams.spectra().FindSpectrum(arg1);
    UpdateSpectrumParamsGUI();
}

void MainWindow::on_leSpectrumName_editingFinished()
{
    UpdateSpectrumParamsData();
}

void MainWindow::on_leSpectrumRed_editingFinished()
{
    UpdateSpectrumParamsData();

}

void MainWindow::on_leSpectrumGreen_editingFinished()
{
    UpdateSpectrumParamsData();
}

void MainWindow::on_leSpectrumBlue_editingFinished()
{
    UpdateSpectrumParamsData();
}

void MainWindow::on_btnNewSpectrum_clicked()
{
    m_curComponentSpectrum = new ComponentSpectrum(QVector3D(1,1,1),"New Spectrum");
    m_renderingParams.spectra().m_spectra.append(m_curComponentSpectrum);
    PopulateSpectraEditList();
    UpdateSpectrumParamsGUI();


}

void MainWindow::on_btnDeleteSpectrum_clicked()
{
    if (m_curComponentSpectrum==nullptr)
        return;

    QMessageBox msgBox;
    msgBox.setText("Are you sure you wish to delete the spectrum '" + m_curComponentSpectrum->name() + "'?");
    msgBox.setInformativeText("This action cannot be undone.");
    msgBox.setStandardButtons(QMessageBox::Ok |  QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();
    if (ret==QMessageBox::Ok) {
        m_renderingParams.spectra().m_spectra.removeAll(m_curComponentSpectrum);
        if (m_renderingParams.spectra().m_spectra.size()!=0)
            m_curComponentSpectrum = m_renderingParams.spectra().m_spectra[0];
        else
            m_curComponentSpectrum = nullptr;

        PopulateSpectraEditList();
        UpdateSpectrumParamsGUI();
    }

}

void MainWindow::on_btnHelpSpectra_clicked()
{
    DialogRendererHelp* dr = new DialogRendererHelp("Spectra", Util::loadTextFile(":/TabSpectraHelp.txt") );
    dr->show();
}

void MainWindow::on_leNoStars_editingFinished()
{
    UpdateStarsData();
}

void MainWindow::on_leBaseSize_editingFinished()
{
    UpdateStarsData();

}

void MainWindow::on_leSpreadSize_editingFinished()
{
    UpdateStarsData();

}

void MainWindow::on_leStarStrength_editingFinished()
{
    UpdateStarsData();

}

void MainWindow::on_btnQueue_clicked()
{
    m_state = State::Queueing;
    Render(true);
}

void MainWindow::on_btnSkybox_clicked()
{
    RenderSkybox();
}


void MainWindow::on_btnSceneMode_clicked()
{
    m_sceneMode = !m_sceneMode;
    if (m_sceneMode)
        m_rasterizer = m_rastScene;
    else
        m_rasterizer = m_rastGalaxy;

    UpdateSceneGUI();
    RenderPreview(m_renderingParams.previewSize());
}

void MainWindow::UpdateSceneGUI()
{
    if (m_sceneMode) {
        ui->btnSceneMode->setText("Galaxy Mode");
        ui->grpGalaxies->setVisible(false);
        ui->grpScene->setVisible(true);
    }
    else {
        ui->btnSceneMode->setText("Scene mode");
        ui->grpGalaxies->setVisible(true);
        ui->grpScene->setVisible(false);
    }

}


void MainWindow::on_btnAddGalaxyToScene_clicked()
{
    if (ui->lstSceneGalaxies->currentItem()==nullptr)
        return;
    ui->lstSceneGalaxiesTo->addItem( ui->lstSceneGalaxies->currentItem()->text());

}

void MainWindow::on_btnAddGalaxyToScene_2_clicked()
{
    for (int i=0;i<ui->lstSceneGalaxies->count();i++)
        ui->lstSceneGalaxiesTo->addItem(ui->lstSceneGalaxies->item(i)->text());

}

void MainWindow::on_btnRemoveGalaxyFromScene_clicked()
{
    if (ui->lstSceneGalaxiesTo->currentItem()==nullptr)
        return;
    delete ui->lstSceneGalaxiesTo->currentItem();

}

void MainWindow::on_btnCreateScene_clicked()
{
    // Create the scene
    m_rasterizer->Clear();
    m_rasterizer->getGalaxies().clear();
    m_galaxies.clear();
    for (int i=0;i<ui->lstSceneGalaxiesTo->count();i++) {
        QString gal = ui->lstSceneGalaxiesTo->item(i)->text();
        Galaxy* g = new Galaxy();
        g->Load(m_renderingParams.galaxyDirectory() + gal);
        m_galaxies.append(g);
    }
    if (m_galaxies.size()==0)
        return;
    int N = ui->leNoGalaxies->text().toInt();
    float boxSize = ui->leBoxSize->text().toFloat();
    for (int i=0;i<N;i++) {
        Galaxy* g = m_galaxies[ rand()%m_galaxies.size() ];
        QVector3D orientation = QVector3D( Random::nextFloat(-1,1), Random::nextFloat(-1,1), Random::nextFloat(-1,1)).normalized();
        QVector3D pos = QVector3D( Random::nextFloat(-1,1), Random::nextFloat(-1,1), Random::nextFloat(-1,1));
        if (i!=0)
            pos*=boxSize;

//        pos = QVector3D(2*i,0,0);

        m_rasterizer->AddGalaxy(new GalaxyInstance(g, g->galaxyParams().name(),
                                                  pos,
                                                  orientation,
                                                   1, 0)  );

    }
    RenderPreview(m_renderingParams.previewSize());

}


void MainWindow::on_cmbRenderer_currentIndexChanged(const QString &arg1)
{
    if (arg1 =="OpenMP") {
        m_rastGalaxy = new Rasterizer(m_rastGalaxy);
        m_rastScene = new Rasterizer(m_rastScene);
    }
    if (arg1=="Threaded"){
        m_rastGalaxy = new RasterThread(m_rastGalaxy);
        m_rastScene = new RasterThread(m_rastScene);
    }
    if (m_sceneMode)
        m_rasterizer = m_rastScene;
    else
        m_rasterizer = m_rastGalaxy;
}


void MainWindow::on_leInner_editingFinished()
{
    UpdateComponentsData();

}

void MainWindow::on_leComponentName_editingFinished()
{
    UpdateComponentsData();
    PopulateCmbComponents();
}
