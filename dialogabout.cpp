#include "dialogabout.h"
#include "ui_dialogabout.h"

DialogAbout::DialogAbout( double version, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    ui->lblVersion->setText("Version " + QString::number(version));
}

DialogAbout::~DialogAbout()
{
    delete ui;
}
