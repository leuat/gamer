#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWindow>
#include <QTimer>
#include "source/galaxy/rasterizer.h"
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/galaxy.h"
#include "source/galaxy/galaxycomponent.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
//    Q_INIT_RESOURCE(gamerresources);


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void loadFile();

    void on_renderButton_clicked();

    void on_actionLoad_triggered();

    void on_actionSave_triggered();

    void on_btnNewComponent_clicked();


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

    void on_cmbImageSize_activated(const QString &arg1);

    void on_leRayStep_editingFinished();

    void on_btnAbort_clicked();

    void on_cmbPreviewSize_activated(const QString &arg1);

    void on_leGalaxyDir_editingFinished();



    void on_lstGalaxies_itemDoubleClicked(QListWidgetItem *item);

    void on_btnNew_clicked();

    void on_leFOV_editingFinished();

    void on_hsExposure_sliderMoved(int position);

    void on_hsGamma_sliderMoved(int position);

    void on_hsSaturation_sliderMoved(int position);

    void on_btnSaveImage_clicked();

    void on_leImageDir_editingFinished();

    void on_leSceneDir_editingFinished();

    void on_btnDelete_clicked();

    void on_btnNewComponent_2_clicked();

    void on_btnClone_clicked();

    void on_pushButton_clicked();

    void on_btnHelpGalaxy_clicked();

    void on_btnHelpComponents_clicked();

private:
    Ui::MainWindow *ui;
    Rasterizer* m_rasterizer = nullptr;
    RenderingParams m_renderingParams;
    Galaxy m_galaxy;
    ComponentParams* m_curComponentParams = nullptr;
    QBasicTimer m_timer;

    float m_postSliderScale = 30;

    void PrepareNewGalaxy();

    QTimer* timer;
    void UpdateImage();

    void PopulateCmbComponents();
    void PopulateCmbComponentTypes();
    void PopulateCmbSpectra();
    void PopulateImageSize();
    void PopulateGalaxyList();

    void UpdateComponentsGUIbyType();

    void UpdateGalaxyGUI();
    void UpdateGalaxyData();
    void UpdateComponentsGUI();
    void UpdateComponentsData();
    void UpdateGUI();
    void UpdateRenderingParamsGUI();
    void UpdateRenderingParamsData();
    void UpdatePostProcessingData();

    void Render();
    void RenderDirect();
    void RenderPreview(int size);

    void EnableGUIEditing(bool value);

    void SaveGalaxy();
    void CreateNewGalaxy();
    QString m_RenderParamsFilename = "RenderParams.dat";

public slots:
   void loop();
};

#endif // MAINWINDOW_H
