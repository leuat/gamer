#include "mainwindow.h"
#include "trianglewindow.h"
#include "window.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);


    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    MainWindow window;

    window.show();


    return app.exec();
}

