#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include "source/galaxy/rasterizer.h"
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadFile();

    void on_renderButton_clicked();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_btnNewComponent_clicked();

    void on_cmbComponentType_currentIndexChanged(const QString &arg1);

    void on_cmbComponentType_activated(const QString &arg1);

    void on_cmbComponents_activated(int index);


    void on_leStrength_editingFinished();

    void on_lePersistence_editingFinished();

    void on_leNoiseTilt_editingFinished();

    void on_leNoiseOffset_editingFinished();

    void on_leScale_editingFinished();

    void on_leWinding_editingFinished();

    void on_leDelta_editingFinished();

    void on_leR0_editingFinished();

    void on_leZ0_editingFinished();

    void on_leArm_editingFinished();

    void on_chkIsActive_clicked();

    void on_cmbSpectrum_activated(const QString &arg1);



    void on_leGalaxyName_editingFinished();

    void on_leWindingB_editingFinished();

    void on_leWindingN_editingFinished();

    void on_leNoArms_editingFinished();

    void on_leArm1_editingFinished();

    void on_leArm2_editingFinished();

    void on_leArm3_editingFinished();

    void on_leArm4_editingFinished();

    void on_actionRender_triggered();

private:
    Ui::MainWindow *ui;
    Rasterizer m_rasterizer;
    Galaxy m_galaxy;
    ComponentParams* m_curComponentParams = nullptr;

    void PrepareNewGalaxy();

    void PopulateCmbComponents();
    void PopulateCmbComponentTypes();
    void PopulateCmbSpectra();

    void UpdateGalaxyGUI();
    void UpdateGalaxyData();
    void UpdateComponentsGUI();
    void UpdateComponentsData();
    void UpdateGUI();

};

#endif // MAINWINDOW_H
