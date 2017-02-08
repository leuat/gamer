#include "simplex.h"

double Simplex::raw_2d(const double& x, const double& y) {
    return raw_noise_2d(x,y);
}

double Simplex::raw_3d(const double& x, const double& y, const double& z) {
    return raw_noise_3d(x,y,z);
}
