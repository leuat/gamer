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
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    m_rasterizer = new Rasterizer(&m_renderingParams);
    ui->myGLWidget->setRenderingParams(&m_renderingParams);
    GMessages::Initialize(ui->lstMessages);
    Spectra::PopulateSpectra();
    PopulateImageSize();
    m_renderingParams.Load(m_RenderParamsFilename);
    UpdateRenderingParamsGUI();

    // Try to load
    m_galaxy.Load(m_renderingParams.galaxyDirectory() + m_renderingParams.currentGalaxy() + ".gax");

    if (m_galaxy.components().size()==0) {
        m_galaxy.AddComponent(10);
        m_renderingParams.setCurrentGalaxy(m_galaxy.galaxyParams().name());
    }


    PrepareNewGalaxy();

    // Adding a single galaxy to the rasterizer
    m_rasterizer->AddGalaxy(new GalaxyInstance(&m_galaxy, m_galaxy.galaxyParams().name(),
                                              QVector3D(0,0,0), QVector3D(0,1,0).normalized(), 1, 0)  );


    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start(10);
    ui->myGLWidget->setRedraw(true);


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
    Render();
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
          ui->cmbComponentType->addItems(QStringList() << "bulge" << "disk" << "dust" << "stars");
        //int idx = ui->cmbComponentType->findData(m_curComponentParams->className());
        ui->cmbComponentType->setCurrentText(m_curComponentParams->className());
    }

}

void MainWindow::PopulateCmbSpectra()
{
    if (m_curComponentParams != nullptr) {
        if (ui->cmbSpectrum->count()==0) {
          ui->cmbSpectrum->addItems(Spectra::listSpectra());
        }
        ui->cmbSpectrum->setCurrentText(m_curComponentParams->spectrum());
    }

}

void MainWindow::PopulateImageSize()
{
    ui->cmbImageSize->clear();
    ui->cmbPreviewSize->clear();
    QStringList l = QStringList() << "16" <<"32" << "48" << "64" << "80" << "96" << "128"
                    << "256" << "384" <<  "512" << "768" << "1024"
                    << "1200" << "1600" << "2048";
    ui->cmbImageSize->addItems(l);
    ui->cmbPreviewSize->addItems(l);
}

void MainWindow::PopulateGalaxyList()
{
    QDirIterator it(m_renderingParams.galaxyDirectory(),
                    QStringList() << "*.gax", QDir::Files, QDirIterator::Subdirectories);
    ui->lstGalaxies->clear();

    while (it.hasNext()) {
        QString filename = it.next();
        QString name = filename.split("/").last();
        ui->lstGalaxies->addItem(name);
    }
}

void MainWindow::UpdateComponentsGUIbyType()
{
    if (m_curComponentParams->className()=="bulge") {
        ui->leStrength->setVisible(true);
        ui->leArm->setVisible(false);
        ui->leZ0->setVisible(false);
        ui->leR0->setVisible(true);
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

    RenderPreview(m_renderingParams.previewSize());
    SaveGalaxy();
}

void MainWindow::UpdateComponentsData() {

    m_curComponentParams->setStrength( ui->leStrength->text().toFloat());
    m_curComponentParams->setArm( ui->leArm->text().toFloat());
    m_curComponentParams->setZ0( ui->leZ0->text().toFloat());
    m_curComponentParams->setR0( ui->leR0->text().toFloat());
    m_curComponentParams->setDelta(ui->leDelta->text().toFloat());
    m_curComponentParams->setWinding( ui->leWinding->text().toFloat());
    m_curComponentParams->setScale( ui->leScale->text().toFloat());
    m_curComponentParams->setNoiseOffset( ui->leNoiseOffset->text().toFloat());
    m_curComponentParams->setNoiseTilt( ui->leNoiseTilt->text().toFloat());
    m_curComponentParams->setKs(ui->lePersistence->text().toFloat());
    m_curComponentParams->setActive(ui->chkIsActive->isChecked()==true);
    m_curComponentParams->setSpectrum(ui->cmbSpectrum->currentText());
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
    ui->leDelta->setText(QString::number(m_curComponentParams->delta()));
    ui->leWinding->setText(QString::number(m_curComponentParams->winding()));
    ui->leScale->setText(QString::number(m_curComponentParams->scale()));
    ui->leNoiseOffset->setText(QString::number(m_curComponentParams->noiseOffset()));
    ui->lePersistence->setText(QString::number(m_curComponentParams->ks()));
    ui->leNoiseTilt->setText(QString::number(m_curComponentParams->noiseTilt()));
    ui->chkIsActive->setChecked(m_curComponentParams->active()==1);
}

void MainWindow::PopulateCmbComponents() {
    int idx = ui->cmbComponents->currentIndex();
    ui->cmbComponents->clear();
    int i = 0;
    for (GalaxyComponent* gc: m_galaxy.components()) {
        QString s = gc->getComponentParams().className() + " " + QString::number(i);
        ui->cmbComponents->addItem(s);
        i++;
    }
    if (idx!=-1)
        ui->cmbComponents->setCurrentIndex(idx);


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

void MainWindow::Render()
{
    EnableGUIEditing(false);
    m_renderingParams.Save(m_RenderParamsFilename);
    m_rasterizer->setNewSize(m_renderingParams.size());
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
    m_renderingParams.setRayStep(0.01);
    m_rasterizer->setNewSize(size);
    RenderDirect();
    m_renderingParams.setSize(oldSize);
    m_renderingParams.setRayStep(oldStep);
}

void MainWindow::EnableGUIEditing(bool value)
{
    ui->myGLWidget->disableInput(!value);
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
//    qDebug() << m_renderingParams.camera().target();
    if (m_rasterizer->getState()==Rasterizer::State::done) {
        //ui->myGLWidget->SetTexture(m_rasterizer->getBuffer());
        UpdateImage();
        EnableGUIEditing(true);
        m_rasterizer->setState(Rasterizer::State::idle);
    }
    if (m_rasterizer->getState()==Rasterizer::State::rendering)
        UpdateImage();

    ui->pgbRendering->setValue((int)(m_rasterizer->getPercentDone()*100.0)+1);

    // Preview image
    if (ui->myGLWidget->redraw()) {
        RenderPreview(m_renderingParams.previewSize());
        m_renderingParams.Save(m_RenderParamsFilename);

    }
    // ETA time
    if (m_rasterizer->getState()==Rasterizer::State::rendering) {
        float time = m_rasterizer->getTimer().elapsed();
        float percentage = m_rasterizer->getPercentDone();
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
    m_curComponentParams->setClassName(arg1);
    ui->cmbComponentType->setCurrentText(arg1);
    m_galaxy.SetupComponents();
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
    m_galaxy.SetupComponents();
    UpdateComponentsGUI();

}



void MainWindow::on_leGalaxyName_editingFinished()
{
    QFile file (m_renderingParams.galaxyDirectory() + m_galaxy.galaxyParams().name() + ".gax");
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
    Render();
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
    QString filename = m_renderingParams.imageDirectory() +
            Util::getFileName(m_renderingParams.imageDirectory(),"Image","png");
    m_rasterizer->getImageShadowBuffer()->save(filename);
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
        QFile file (filename);
        file.remove();
        PopulateGalaxyList();
    }
}

void MainWindow::on_btnNewComponent_2_clicked()
{
    if (m_galaxy.componentParams().size()<=1)
        return;
    m_galaxy.componentParams().removeAll(m_curComponentParams);
    m_galaxy.SetupComponents();
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
