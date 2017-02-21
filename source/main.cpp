#include "mainwindow.h"
#include "trianglewindow.h"
#include "window.h"
#include "source/consolerenderer.h"
#include <stdio.h>
#include <QApplication>
#include <QDebug>

using namespace std;

int main(int argc, char *argv[])
{

    if (argc==1) {

        QApplication app(argc, argv);
        MainWindow window;
        window.show();
        return app.exec();
    }
    else {
        ConsoleRenderer cr(argc, argv);
    }
}

