#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include "source/glwidget.h"

//class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

private slots:
    void setCurrentGlWidget();
   // void rotateOneStep();

private:
    enum { NumRows = 1, NumColumns = 2 };

    GLWidget *glWidgets[NumRows][NumColumns];
    GLWidget *currentGlWidget;
};

#endif
