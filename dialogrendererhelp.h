#ifndef DIALOGRENDERERHELP_H
#define DIALOGRENDERERHELP_H

#include <QDialog>

namespace Ui {
class DialogRendererHelp;
}

class DialogRendererHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRendererHelp(QString header, QString text, QWidget *parent = 0);
    ~DialogRendererHelp();

private:
    Ui::DialogRendererHelp *ui;
};

#endif // DIALOGRENDERERHELP_H
