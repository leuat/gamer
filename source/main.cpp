#include "mainwindow.h"
#include "trianglewindow.h"
#include "window.h"
#include "source/consolerenderer.h"
#include <stdio.h>
#include <QApplication>
#include <QDebug>

using namespace std;



void SetDarkPalette() {
    QPalette darkPalette = QApplication::palette();
    darkPalette.setColor(QPalette::Window, QColor(53,53,53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(253,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    QApplication::setPalette(darkPalette);
    qApp->setStyleSheet("QToolTip { color: #ffE0C0; background-color: #000000; border: 0px; }");


}


int main(int argc, char *argv[])
{
  if (argc==1) {

        QApplication app(argc, argv);
        MainWindow window;
        window.show();
//        SetDarkPalette();
        return app.exec();
    }
    else {
        ConsoleRenderer cr(argc, argv);
    }
}

