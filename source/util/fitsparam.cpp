#include "fitsparam.h"
#include <QDebug>

FitsParam::FitsParam(FitsHeader* fh, int pos) {
    QString d = "";
    for (int i=pos;i<pos+80;i++) {
            if (fh->data[i]=='/')
                break;
            if (fh->data[i]!=' ')
                d+=QChar(fh->data[i]);
        }
        d = d.trimmed();
        if (!d.contains("=")) {
            name ="";
            value="";
            return;
        }
        QStringList v = d.split('=');

        name = v[0];
        value = v[1];
    }

QString FitsHeader::getParam(QString name) {
    for (FitsParam fp : param)
        if (fp.name.toLower() == name.toLower())
            return fp.value;
    return "0";
}
double FitsHeader::getParamdouble(QString name) {
    return getParam(name).toFloat();
}
void FitsHeader::ExtractParams() {
    param.clear();
    for (int i=0;i<2880/80;i++) {
        FitsParam fp(this, i*80);
        if (fp.name != "")
            param.append(fp);
        if (fp.name.toLower().contains("end"))
            isEnd = true;

    }
}

FitsHeader::FitsHeader() {
    for (int i=0;i<Length;i++)
        data[i] = ' ';
}

void FitsHeader::Add(QString name, QString value) {
    int cur = curLine*80;
    for (int i=0;i<name.length();i++)
        data[i + cur] = name[i].toLatin1();

    cur += name.length();

    for (int i=0;i<value.length();i++) {
        data[i + cur] = value[i].toLatin1();
    }

    curLine++;

}
