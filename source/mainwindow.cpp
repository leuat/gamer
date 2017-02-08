#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QStringList>
#include <QMessageBox>
#include "source/galaxy/spectrum.h"
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->myGLWidget->setRenderingParams(&m_rasterizer.getRenderingParams());
    Spectra::PopulateSpectra();
    PopulateImageSize();
    m_galaxy.AddComponent();
    PrepareNewGalaxy();
    // Adding a single galaxy to the rasterizer
    m_rasterizer.AddGalaxy(new GalaxyInstance(&m_galaxy, m_galaxy.galaxyParams().name(),
                                              QVector3D(0,0,0), QVector3D(0,1,0).normalized(), 1, 0)  );
    m_rasterizer.getRenderingParams().Load(m_RenderParamsFilename);
    UpdateRenderingParamsGUI();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(loop()));
    timer->start(5);
}

MainWindow::~MainWindow()
{
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
    if (!m_galaxy.Load("test.gax")) {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.exec();
        return;
    }
    PrepareNewGalaxy();

}

void MainWindow::on_actionSave_triggered()
{
    m_galaxy.Save("test.gax");
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
    ui->cmbImageSize->addItems(QStringList() << "16" <<"32" << "64" << "100" << "128"
                               << "256" << "400" <<  "512" << "768" << "1024"
                               << "1200" << "1600" << "2048" );
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

}

void MainWindow::UpdateComponentsGUI()
{
    PopulateCmbComponentTypes();
    PopulateCmbSpectra();
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
//    qDebug() << m_rasterizer.getRenderingParams().size();
    ui->cmbImageSize->setCurrentText(QString::number(m_rasterizer.getRenderingParams().size()));
    ui->leRayStep->setText( QString::number(m_rasterizer.getRenderingParams().rayStep()));
}

void MainWindow::UpdateRenderingParamsData()
{
    m_rasterizer.getRenderingParams().setSize(ui->cmbImageSize->currentText().toInt());
    m_rasterizer.getRenderingParams().setRayStep(ui->leRayStep->text().toFloat());
    m_rasterizer.setNewSize(m_rasterizer.getRenderingParams().size());
    m_rasterizer.getRenderingParams().Save(m_RenderParamsFilename);
}

void MainWindow::Render()
{
    m_rasterizer.getRenderingParams().Save(m_RenderParamsFilename);
    QElapsedTimer timer;
    timer.start();
    m_rasterizer.Render();
    qDebug() << "Rendering took " << timer.elapsed()/1000.0 << " seconds";
    ui->myGLWidget->SetTexture(m_rasterizer.getBuffer());

}

void MainWindow::loop()
{
//    qDebug() << "WHY NOT CALL ME DUDE";
//    qDebug() << "other: " << ui->myGLWidget->redraw();
    if (ui->myGLWidget->redraw()) {
        int oldSize = m_rasterizer.getRenderingParams().size();
        m_rasterizer.setNewSize(64);
        Render();
        m_rasterizer.setNewSize(oldSize);
        m_rasterizer.getRenderingParams().Save(m_RenderParamsFilename);

    }
}

void MainWindow::on_btnNewComponent_clicked()
{
    m_curComponentParams = m_galaxy.AddComponent();
    UpdateGUI();
    UpdateComponentsGUI();
}

void MainWindow::on_cmbComponentType_currentIndexChanged(const QString &arg1)
{
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
    UpdateGalaxyData();
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
