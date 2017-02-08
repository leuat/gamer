#pragma once
#include <QObject>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <QVector>
#include <QVector3D>

using namespace std;

/*
 * Parent class for noise modules. Currently only implemented as perlin & simplex noise.
 *
 * must implement raw_2d & raw_3d
 *
 *
*/
class Noise : public QObject {
    Q_OBJECT
    Q_ENUMS(NoiseType)
public:
    int octaves, seed;
    double persistence;
    double frequency;
    double average;
    double standard_deviation;
    double* weights = nullptr;

    enum NoiseType { Simplex, Perlin};

    Noise(int o,double f,double p,int s);

    double get( const double x, const double y, const double z);
    double get_raw( const double x, const double y, const double z);
    double get(const double x, const double y, const double z, const double per, const double f);
    double get( const double x, const double y);
    double* generateSpectralWeights(double lacunarity, int octaves, double h, double frequency);
    double getRidgedMf(QVector3D p, float frequency, int octaves, float lacunarity, float warp, float offset, float gain);
    void calculate_statistics(double N, string filename);

    virtual double raw_2d(const double& x, const double& y) = 0;
    virtual double raw_3d(const double& x, const double& y, const double& z) = 0;
};
