#include "dialogrendererhelp.h"
#include "ui_dialogrendererhelp.h"

DialogRendererHelp::DialogRendererHelp(QString header, QString text,QWidget *parent ) :
    QDialog(parent),
    ui(new Ui::DialogRendererHelp)
{
    ui->setupUi(this);
    ui->label->setText(header);
    ui->textBrowser->setText(text);
}

DialogRendererHelp::~DialogRendererHelp()
{
    delete ui;
}
