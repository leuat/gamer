#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glwidget.h"
#include <QDebug>
#include <QGridLayout>
#include <QStringList>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

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
    m_rasterizer.Render();
    ui->myGLWidget->SetTexture(m_rasterizer.getBuffer());
}

void MainWindow::on_actionLoad_triggered()
{
    if (!m_galaxy.Load("test.gax")) {
        QMessageBox msgBox;
        msgBox.setText("Could not open file");
        msgBox.exec();
        return;
    }
    if (m_galaxy.componentParams().size()!=0)
        m_curComponentParams = m_galaxy.componentParams()[0];
    UpdateGUI();
    UpdateComponentsGUI();

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

void MainWindow::UpdateComponentsData() {
    m_curComponentParams->setStrength( ui->leStrength->text().toFloat());

}

void MainWindow::UpdateComponentsGUI()
{
    PopulateCmbComponentTypes();
    ui->leStrength->setText(QString::number(m_curComponentParams->strength()));

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

void MainWindow::on_leStrength_returnPressed()
{
    UpdateComponentsData();
}
