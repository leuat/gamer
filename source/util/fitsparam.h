#pragma once

#include <QString>
#include <QVector>

class FitsHeader;

class FitsParam {
public:
    QString value;
    QString name;

    FitsParam(FitsHeader* fh, int pos);
    FitsParam() {

    }

};

class FitsHeader {
public:
        const int Length = 2880;
        int curLine = 0;
        unsigned char* data = new unsigned char[Length];
        QVector<FitsParam> param;// = new List<FitsParam>();
        bool isEnd = false;

        QString getParam(QString name);
        float getParamFloat(QString name);
        void ExtractParams();
        FitsHeader();
        void Add(QString name, QString value);


    };
