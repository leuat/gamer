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

    void on_leStrength_returnPressed();

private:
    Ui::MainWindow *ui;
    Rasterizer m_rasterizer;
    Galaxy m_galaxy;
    ComponentParams* m_curComponentParams = nullptr;

    void PopulateCmbComponents();
    void PopulateCmbComponentTypes();
    void UpdateComponentsGUI();
    void UpdateComponentsData();
    void UpdateGUI();

};

#endif // MAINWINDOW_H
