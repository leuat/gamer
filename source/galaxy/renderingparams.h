#pragma once

#include <QVector3D>
#include <QString>
#include "source/galaxy/gamercamera.h"

class RenderingParams {
public:
    int size() const;
    void setSize(int size);

    GamerCamera& camera();
    void setCamera(const GamerCamera &camera);

    float rayStep() const;
    void setRayStep(float rayStep);

    int m_size = 128; // pixels
    int m_previewSize = 64;
    float m_exposure = 1;
    float m_gamma = 1;
    float m_saturation = 1;
    float m_detailLevel = 0.01;
    float m_noiseDetail = 1;
    int m_noStars=0;
    float m_starSize=1, m_starSizeSpread=1;
    float m_starStrength = 1;
    float m_rayStep = 0.001;
    QVector3D color = QVector3D(1,1,1);
    QString m_galaxyDirectory = "galaxies/";
    QString m_sceneDirectory = "scenes/";
    QString m_currentGalaxy ="";
    QString m_sceneMode ="galaxy";

    friend QDataStream& operator << ( QDataStream & s, RenderingParams& cp) {
        s << cp.camera();
        s << cp.m_size << cp.m_previewSize << cp.m_exposure << cp.m_gamma << cp.m_saturation << cp.m_detailLevel;
        s << cp.m_noiseDetail << cp.m_noStars << cp.m_starSize << cp.m_starSizeSpread;
        s << cp.m_starStrength << cp.m_rayStep << cp.m_galaxyDirectory << cp.m_sceneDirectory << cp.m_currentGalaxy << cp.m_sceneMode;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, RenderingParams& cp) {
        GamerCamera camera;
        s >> camera;
        cp.setCamera(camera);
        s >> cp.m_size >> cp.m_previewSize >> cp.m_exposure >> cp.m_gamma >> cp.m_saturation >> cp.m_detailLevel;
        s >> cp.m_noiseDetail >> cp.m_noStars >> cp.m_starSize >> cp.m_starSizeSpread;
        s >> cp.m_starStrength >> cp.m_rayStep >> cp.m_galaxyDirectory >> cp.m_sceneDirectory >> cp.m_currentGalaxy;
        s >> cp.m_sceneMode;
        return s;
    }
    void Save(QString filename);
    void Load(QString filename);

    int previewSize() const;
    void setPreviewSize(int previewSize);

    QString galaxyDirectory() const;
    void setGalaxyDirectory(const QString &galaxyDirectory);

    QString sceneDirectory() const;
    void setSceneDirectory(const QString &sceneDirectory);

    QString currentGalaxy() const;
    void setCurrentGalaxy(const QString &currentGalaxy);

private:
    float m_idx;
    float m_idxCount;
    float m_divide;
    GamerCamera m_camera;// = new GamerCamera();
    QVector3D direction;

    float m_seed;
    //int no_procs;

    //bool continuousPostprocessing=true;


    QString m_currentScene = "";

    float m_rayStepPreview;
    float m_rayStepNormal;
    float m_wavelength;

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


