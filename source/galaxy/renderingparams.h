#pragma once

#include <QVector3D>
#include <QString>
#include "source/galaxy/spectrum.h"
#include "source/galaxy/gamercamera.h"

class RenderingParams {
public:
    int size() const;
    void setSize(int size);

    GamerCamera& camera();
    void setCamera(const GamerCamera &camera);

    double rayStep() const;
    void setRayStep(double rayStep);

    int m_size = 128; // pixels
    int m_nside = 32;
    QString m_renderType;
    int m_previewSize = 64;
    double m_exposure = 1;
    double m_gamma = 1;
    double m_saturation = 1;
    double m_detailLevel = 0.01;
    double m_noiseDetail = 1;
    int m_noStars=0;
    double m_starSize=1, m_starSizeSpread=1;
    double m_starStrength = 1;
    double m_rayStep = 0.001;
    QVector3D color = QVector3D(1,1,1);
    QString m_galaxyDirectory = "galaxies/";
    QString m_sceneDirectory = "scenes/";
    QString m_imageDirectory = "images/";
    QString m_currentGalaxy ="";
    QString m_sceneMode ="galaxy";

    Spectra m_spectra;

    friend QDataStream& operator << ( QDataStream & s, RenderingParams& cp) {
        s << cp.camera();
        s << cp.m_size << cp.m_previewSize << cp.m_exposure << cp.m_gamma << cp.m_saturation << cp.m_detailLevel;
        s << cp.m_noiseDetail << cp.m_noStars << cp.m_starSize << cp.m_starSizeSpread;
        s << cp.m_starStrength << cp.m_rayStep << cp.m_galaxyDirectory << cp.m_sceneDirectory << cp.m_currentGalaxy << cp.m_sceneMode;
        s << cp.m_imageDirectory;
        s << cp.m_spectra;
        s << cp.m_nside << cp.m_renderType;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, RenderingParams& cp) {
        GamerCamera camera;
        s >> camera;
        cp.setCamera(camera);
        s >> cp.m_size >> cp.m_previewSize >> cp.m_exposure >> cp.m_gamma >> cp.m_saturation >> cp.m_detailLevel;
        s >> cp.m_noiseDetail >> cp.m_noStars >> cp.m_starSize >> cp.m_starSizeSpread;
        s >> cp.m_starStrength >> cp.m_rayStep >> cp.m_galaxyDirectory >> cp.m_sceneDirectory >> cp.m_currentGalaxy;
        s >> cp.m_sceneMode >> cp.m_imageDirectory;
        s >> cp.m_spectra;
        s >> cp.m_nside >> cp.m_renderType;

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

    double exposure() const;
    void setExposure(double exposure);

    double gamma() const;
    void setGamma(double gamma);

    double saturation() const;
    void setSaturation(double saturation);

    int noStars() const;
    void setNoStars(int noStars);

    double starSize() const;
    void setStarSize(double starSize);

    double starSizeSpread() const;
    void setStarSizeSpread(double starSizeSpread);

    double starStrength() const;
    void setStarStrength(double starStrength);

    QString imageDirectory() const;
    void setImageDirectory(const QString &imageDirectory);

    Spectra& spectra();

    int nside() const;
    void setNside(int nside);

    QString renderType() const;
    void setRenderType(const QString &renderType);

private:
    double m_idx;
    double m_idxCount;
    double m_divide;
    GamerCamera m_camera;// = new GamerCamera();
    QVector3D direction;

    double m_seed;
    //int no_procs;

    //bool continuousPostprocessing=true;


    QString m_currentScene = "";

    double m_rayStepPreview;
    double m_rayStepNormal;
    double m_wavelength;

    double m_bufferSize;

    double m_randomize_spectra;



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


