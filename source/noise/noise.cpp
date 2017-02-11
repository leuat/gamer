#include "noise.h"

Noise::Noise(int o, double f, double p, int s) {
    octaves = o;
    persistence = p;
    frequency = f;
    seed = s;
    weights = 0;
}

double Noise::get( const double x, const double y, const double z ) {
    return get(x,y,z, persistence, frequency);
}


double Noise::get( const double x, const double y, const double z, const double per, const double f) {
    double total = 0;

    double freq = f;
    double amplitude = 1.0;
    double p = per;
    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    double maxAmplitude = 0;
//    A*p^()


    for( int i=0; i < octaves; i++ ) {
        double v = raw_3d( (x+0.1231*i) * freq, (y+0.6123*i) * freq, (z+100.539127*i) * freq );
//        double v = raw_3d( (x) * freq, (y) * freq, (z) * freq );

        total += v*amplitude;
        freq *= 2;
        maxAmplitude += amplitude;
        amplitude *= p;
        //p = sqrt(p);

    }

    return total / maxAmplitude;
}

double Noise::get_raw( const double x, const double y, const double z) {
    return raw_3d( (x), (y) , (z) );
}


double Noise::get( const double x, const double y) {
    double total = 0;
    double freq = frequency;
    double amplitude = 1;

    double maxAmplitude = 0;

    for( int i=0; i < octaves; i++ ) {
        total += raw_2d( x * freq, y * freq) * amplitude;

        freq *= 2;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}

double* Noise::generateSpectralWeights (double lacunarity,
                                        int octaves, double h, double frequency) {
    double* weights = new double[octaves];

    for (int octave = 0; octave < octaves; ++octave) {
        weights[octave] = pow (frequency, h);
        frequency *= lacunarity;
    }

    return weights;
}


//
// Not really used, but could be useful for nice dust patterns
//
double Noise::getRidgedMf(QVector3D p, float frequency, int octaves, float lacunarity, float offset, float gain)
{
    double value = 0.0;
    double weight = 1.0;

    double w = -0.05f;

    if (weights == 0)
        weights = generateSpectralWeights(lacunarity, octaves, w, frequency);

    QVector3D vt = p;
    for (int octave = 0; octave < octaves; octave++) {
        double signal = raw_3d(vt.x(), vt.y(), vt.z());

        // Make the ridges.
        signal = fabs (signal);
        signal = offset - signal;

        // Square the signal to increase the sharpness of the ridges.
        signal *= signal;

        // The weighting from the previous octave is applied to the signal.
        // Larger values have higher weights, producing sharp points along the
        // ridges.
        signal *= weight;

        // Weight successive contributions by the previous signal.
        weight = signal * gain;
        if (weight > 1.0) {
            weight = 1.0;
        }
        if (weight < 0.0) {
            weight = 0.0;
        }

        // Add the signal to the output value.


        value += (signal * pow (frequency, w));
        //value += (signal * weights[octave]);

        // Go to the next octave.
        vt = vt * lacunarity;
        frequency *= lacunarity;
    }

    return (value * 1.25) - 1.0;
}



void Noise::calculate_statistics(double N, std::string filename) {
    average = 0;
    double s = 0.01123;
    standard_deviation = 0;
    for (int i=0;i<N;i++) {

        double val = get(i*s+0.1, i*s+0.211, i*s+0.123);
        average += val;
    }
    average /= N;
    for (int i=0;i<N;i++) {
        double val = get(i*s+0.1, i*s+0.211, i*s+0.123);
        standard_deviation += (average-val)*(average-val);
    }
    standard_deviation = sqrt(standard_deviation/N);

    if (filename != "") {
        ofstream f( filename.c_str(), ios::out);
        for (int i=0;i<50000;i++) {
            double s = 0.159516413;
            double val = get((double)i*s, 0.121+i*s*0.341,0.4312+i*s*0.1231 + 0.64123);
            f << i << " " << val << endl;
        }
        f.close();


    }

}

double Noise::octave_noise_3d(const double octaves, const double persistence, const double scale, const double x, const double y, const double z)
{
    double total = 0;
    double frequency = scale;
    double amplitude = 1;
    // We have to keep track of the largest possible amplitude,
    // because each octave adds more, and we need a value in [-1, 1].
    double maxAmplitude = 0;

    for( int i=0; i < octaves; i++ ) {
        total += raw_3d( x * frequency, y * frequency, z * frequency ) * amplitude;

        frequency *= 2;
        maxAmplitude += amplitude;
        amplitude *= persistence;
    }

    return total / maxAmplitude;
}


