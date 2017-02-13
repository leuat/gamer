#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <random>
#include <QVector3D>
#include <QImage>
#include <QString>
#include <QRgb>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QTextStream>
#include <QDebug>

#define Q_TIMER_START()  \
{                        \
    QElapsedTimer ttimer; \
    ttimer.start();        \

#define Q_TIMER_ELAPSED(name) \
   qDebug() << (name) << " took "<< QString::number(ttimer.elapsed()/1000.0) << " seconds"; \
}


using namespace std;

class Util {

public:
    static void Tokenize(const string& str,
                         vector<string>& tokens,
                         const string& delimiters = " ");


    static void string2char(string s, char* to);
    static string toString(double d, string param);
    static string toString(double d);
    static string toString(int d);
    static const char* read_textfile(string filename);
    static void verify_file(string filename);
    static bool verify_file_bool(string filename);
    static string trim(string s);

    static float floatRandom(const float & min, const float & max) {
        static std::mt19937 generator;
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }

    static bool IntersectSphere(QVector3D o, QVector3D d, QVector3D r,QVector3D& isp1,QVector3D& isp2, float& t0, float& t1) {

        r.setX(1.0/(r.x()*r.x()));
        r.setY(1.0/(r.y()*r.y()));
        r.setZ(1.0/(r.z()*r.z()));


        QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
        QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


        float A = QVector3D::dotProduct(d,rD);
        float B = 2.0*(QVector3D::dotProduct(d, rO));
        float C = QVector3D::dotProduct(o, rO) - 1.0;

        float S = (B*B - 4.0f*A*C);

        if (S<=0) {
            isp1 = QVector3D(0,0,0);
            isp2 = QVector3D(0,0,0);
            t0 = 0;
            t1 = 0;
            return false;
        }

        t0 =  (-B - sqrt(S))/(2*A);
        t1 =  (-B + sqrt(S))/(2*A);

        isp1 = o+d*t0;
        isp2 = o+d*t1;

        return true;
    }

    static float smoothstep(float edge0, float edge1, float x);
    static float clamp(float val, float min, float max);
    static QVector3D clamp(QVector3D val, float min, float max);

    static void drawBox(QImage* backImage, QImage* img, int i, int j, int size, QRgb color) {
        int imageSize = img->width();
        QRgb mark = QColor(1,1,1).rgba();
        for (int x=max(0, i-size/2);x<=min(imageSize-1, i+size/2);x++)
            for (int y=max(0, j-size/2);y<=min(imageSize-1, j+size/2);y++) {
                QColor col = QColor::fromRgba(backImage->pixel(x,y));
                if (col.red()==0) {
                    img->setPixel(x,y,color);
                    if (x==i && y== j)
                        backImage->setPixel(x,y,mark);
                }
            }
    }

    template<typename T>
    static QVector<T> ShuffleArray(QVector<T> a)
    {
        for (int i = a.size(); i > 0; i--)
        {
            T j = random()%i;// r.Next(i);
            T k = a[j];
            a[j] = a[i - 1];
            a[i - 1]  = k;
        }
        return a;
    }

    static QString getFileName(QString dir, QString baseName, QString type);

    static QString loadTextFile(QString filename) {
        QFile file(filename);
        file.open(QIODevice::ReadOnly);
        QTextStream in(&file);
        QString data = in.readAll();
        file.close();
        return data;

    }


};

