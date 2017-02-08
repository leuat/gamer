#pragma once

#include <QVector3D>
#include <QString>
#include "source/galaxy/gamercamera.h"

class RenderingParams {
public:
    int size() const;
    void setSize(int size);

    GamerCamera camera() const;
    void setCamera(const GamerCamera &camera);

    float rayStep() const;
    void setRayStep(float rayStep);

private:
    float m_idx;
    float m_idxCount;
    float m_divide;
    GamerCamera m_camera;// = new GamerCamera();
    QVector3D direction;

    float m_detailLevel = 0.01;
    float m_noiseDetail = 1;
    float m_seed;
    int m_size = 64; // pixels
    float m_exposure = 1;
    float m_gamma = 1;
    float m_saturation = 1;
    //int no_procs;

    //bool continuousPostprocessing=true;

    int m_noStars=0;
    float m_starSize=1, m_starSizeSpread=1;
    float m_starStrength = 1;

    QString m_currentScene = "";

    float m_rayStep;
    float m_rayStepPreview;
    float m_rayStepNormal;
    QVector3D color = QVector3D(1,1,1);
    float m_wavelength;

    QString m_currentGalaxy = "";
    float m_bufferSize;

    float m_randomize_spectra;



    /*		static void Save(string filename, RenderingParams p) {
            XmlSerializer serializer = new XmlSerializer(typeof(RenderingParams));
            TextWriter textWriter = new StreamWriter(filename);
            serializer.Serialize(textWriter, p);
            textWriter.Close();

        }
        public static RenderingParams Load(string filename) {
            XmlSerializer deserializer = new XmlSerializer(typeof(RenderingParams));
            TextReader textReader = new StreamReader(filename);
            RenderingParams p = (RenderingParams)deserializer.Deserialize(textReader);
            textReader.Close();
            return p;
        }

*/
    };


