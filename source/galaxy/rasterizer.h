#pragma once

#include <QVector>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QElapsedTimer>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/rasterpixel.h"
#include "source/noise/simplexnoise.h"
#include "source/util/gmessages.h"
#include "source/noise/noise.h"
#include "source/noise/simplex.h"
#include "source/noise/iqnoise.h"
#include "source/util/buffer2d.h"

class Rasterizer : public QThread {
public:
    enum State { idle, rendering, done, aborting };


private:
    QVector<GalaxyInstance*> m_galaxies;
    RenderingParams* m_renderingParams;

    QVector<int> m_renderList;
    QImage* m_imageBuffer = nullptr;
    QImage* m_imageShadowBuffer = nullptr;
    // Backbuffer used to mark whether pixel is set or not
    Buffer2D* m_backBuffer = nullptr;
    Buffer2D* m_renderBuffer = nullptr;
    Buffer2D* m_starsBuffer = nullptr;

    bool m_restart = false;
    bool m_abort = false;
    State m_state = State::idle;

    Noise* m_noise = nullptr;

    bool m_isPreview = false;

    QMutex m_mutex;
    QWaitCondition m_condition;

    float m_percentDone = 0;
    QElapsedTimer m_timer;

protected:
    void run() override;

 public:

    Rasterizer() {
        m_noise = new Simplex(1,1,1,1);
    }
    Rasterizer(RenderingParams* rp) {
        m_renderingParams = rp;
//        m_noise = new IQnoise(1,1,1,1);
        m_noise = new Simplex(1,1,1,1);
    }

    ~Rasterizer() {
        m_mutex.lock();
        m_abort = true;
        m_condition.wakeOne();
        m_mutex.unlock();
        wait();
        ReleaseBuffers();
    }

    void ReleaseBuffers();

    void prepareRenderList();
    void setNewSize(int s);
    void prepareBuffer();
    void Prepare();
    void RenderStars();


    void Abort() {
        m_abort = true;
        requestInterruption();
        quit();
        wait();
        m_state = State::aborting;
        GMessages::Message("Aborting threaded rendering!");
    }
    Galaxy* AddGalaxy(QString file, QVector3D position, QVector3D orientation, float iscale, float redshift, QString name);

    Galaxy* AddGalaxy(GalaxyInstance* gi);

    void CopyFrom(Rasterizer* from);
    void RenderPixels();
    void AssembleImage();
    void Render();
    void RenderDirect();
    void InitializeRendering();
    QVector3D setupCamera(int idx);
    QVector3D renderPixel(QVector3D dir, QVector<GalaxyInstance*> gals);
    void getIntensity(GalaxyInstance* gi, RasterPixel* rp, QVector3D isp1, QVector3D isp2);




		
    QImage *getBuffer() const;
    RenderingParams* getRenderingParams();
    State getState();
    void setState(const State &state);
    float getPercentDone() const;
    QImage *getImageShadowBuffer() const;
    QElapsedTimer getTimer() const;
    Buffer2D *getRenderBuffer() const;
    void setRenderingParams(RenderingParams *renderingParams);
    QVector<GalaxyInstance *> getGalaxies() const;
    bool getAbort() const;
};


