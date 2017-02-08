#include "window.h"

#include <QtWidgets>

#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"
#include <qdebug.h>

Window::Window()
{
    qDebug() << "GL";
    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addWidget(new GLWidget(), 0, 0);
    mainLayout->addWidget(new GLWidget(), 0, 1);
    MainWindow* mw = new MainWindow();
    mainLayout->addWidget(mw, 0, 1);
    setLayout(mainLayout);
    mw->show();


    setWindowTitle(tr("Gamer 2.0"));
}

void Window::setCurrentGlWidget()
{
 //   currentGlWidget = qobject_cast<GLWidget *>(sender());
}

