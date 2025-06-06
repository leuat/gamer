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
#define M_PI 3.14159265

//#include "random.h"

#define Q_TIMER_START()  \
{                        \
    QElapsedTimer ttimer; \
    ttimer.start();        \

#define Q_TIMER_ELAPSED(name) \
   qDebug() << (name) << " took "<< QString::number(ttimer.elapsed()/1000.0) << " seconds"; \
}



class Util {

public:
    static void Tokenize(const std::string& str,
                         std::vector<std::string>& tokens,
                         const std::string& delimiters = " ");


    static void string2char(std::string s, char* to);
    static std::string toString(double d, std::string param);
    static std::string toString(double d);
    static std::string toString(int d);
    static const char* read_textfile(std::string filename);
    static void verify_file(std::string filename);
    static bool verify_file_bool(std::string filename);
    static std::string trim(std::string s);
    static QString path;
    static double doubleRandom(const double & min, const double & max) {
        static std::mt19937 generator;
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(generator);
    }

    static QVector3D fromSpherical(double r, double t, double p) {
        return QVector3D( r*sin(t)*cos(p), r*sin(t)*sin(p), r*cos(t)  );
    }

    static QVector3D floor(const QVector3D v) {
        return QVector3D( fmax(0.0f, v.x()), fmax(0.0f,v.y()), fmax(0.0f,v.z())  );
    }

    static bool IntersectSphere(QVector3D o, QVector3D d, QVector3D r,QVector3D& isp1,QVector3D& isp2, double& t0, double& t1) {

        r.setX(1.0/(r.x()*r.x()));
        r.setY(1.0/(r.y()*r.y()));
        r.setZ(1.0/(r.z()*r.z()));


        QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
        QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


        double A = QVector3D::dotProduct(d,rD);
        double B = 2.0*(QVector3D::dotProduct(d, rO));
        double C = QVector3D::dotProduct(o, rO) - 1.0;

        double S = (B*B - 4.0f*A*C);

        if (S<=0) {
            isp1 = QVector3D(0,0,0);
            isp2 = QVector3D(0,0,0);
            t0 = 0;
            t1 = 0;
            return false;
        }

        t0 =  (-B - sqrt(S))/(2.0*A);
        t1 =  (-B + sqrt(S))/(2.0*A);

        isp1 = o+d*t0;
        isp2 = o+d*t1;

        return true;
    }

    static double smoothstep(double edge0, double edge1, double x);
    static double clamp(double val, const double min, const double max);
    static QVector3D clamp(QVector3D val, double min, double max);

    static void drawBox(QImage* backImage, QImage* img, int i, int j, int size, QRgb color) {
        int imageSize = img->width();
        QRgb mark = QColor(1,1,1).rgba();
        for (int x=fmax(0, i-size/2);x<=fmin(imageSize-1, i+size/2);x++)
            for (int y=fmax(0, j-size/2);y<=fmin(imageSize-1, j+size/2);y++) {
                QColor col = QColor::fromRgba(backImage->pixel(x,y));
                if (col.red()==0) {
                    img->setPixel(x,y,color);
                    if (x==i && y== j)
                        backImage->setPixel(x,y,mark);
                }
            }
    }

/*    static QVector<int> ShuffleArray(QVector<int>& a)
    {
        for (int i = a.size(); i > 0; i--)
        {
            int j = rand()%i;// r.Next(i);
            int k = a[j];
            a[j] = a[i - 1];
            a[i - 1]  = k;
        }
        return a;
    }
*/
    template<typename T>
    static QVector<T> ShuffleArray(QVector<T> a)
    {
        for (int i = a.size(); i > 0; i--)
        {
            int j = rand()%i;// r.Next(i);
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

    static QString MilisecondToString(int ms) {
//        ms+=1000;
        int ds = ms/100;
        int s = (ms/1000);
        int m = (s/60);
        int h = (m/60);
        int d = h/24;
        ds = ds % 10;
        s = s % 60;
        m = m % 60;
        h = h % 24;
        QString str = "";
        if (d!=0)
            str+= QString::number(d) + "d ";
        if (h!=0)
            str+= QString::number(h) + "h ";
        if (m!=0)
            str+= QString::number(m) + "m ";
        str+= QString::number(s) + "." + QString::number(ds) + "s ";
        return str;
    }
    static QVector3D maxQvector3D(const QVector3D a, const QVector3D b) {
        return QVector3D(fmax(a.x(), b.x()),fmax(a.y(), b.y()),fmax(a.z(), b.z()));
    }

    inline static bool Mollweide(QVector3D& out, double i, double j, double l0, double R, double size) {

        double x = 4*R*sqrt(2)*(2*i/(double)size-1);
        double yy = j*2 - size/2;
        double y = R*sqrt(2)*(2*yy/(double)size-1);


        double t = asin(y/(R*sqrt(2.0)));
        out = QVector3D( asin( (2.0*t + sin(2.0*t))/M_PI),l0 + M_PI*x / (2*R*sqrt(2.0)*cos(t)),0  );
        out.setX(out.x()+M_PI/2);
        out.setY(-out.y()*0.5);
        if (out.y()>-M_PI && out.y()<M_PI)
            return true;

        return false;

    }

};

template <typename T>
struct PtrLess // public std::binary_function<bool, const T*, const T*>
{
  bool operator()(const T* a, const T* b) const
  {
    // may want to check that the pointers aren't zero...
    return *a < *b;
  }
};
