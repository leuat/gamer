#pragma once

#include <QVector>
#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/rasterpixel.h"
#include "source/noise/simplexnoise.h"
#include "source/util/gmessages.h"

class Rasterizer : public QThread {
public:
    enum State { idle, rendering, done };


private:
    QVector<GalaxyInstance*> m_galaxies;
    RenderingParams* m_renderingParams;
    QVector<int> m_renderList;
    QImage* m_buffer = nullptr;
    // Backbuffer used to mark whether pixel is set or not
    QImage* m_backBuffer = nullptr;
    bool m_restart = false;
    bool m_abort = false;
    State m_state = State::idle;

    QMutex m_mutex;
    QWaitCondition m_condition;

    float m_percentDone = 0;

protected:
    void run() override;

 public:


    Rasterizer(RenderingParams* rp) {
        m_renderingParams = rp;
    }
    ~Rasterizer() {
        m_mutex.lock();
        m_abort = true;
        m_condition.wakeOne();
        m_mutex.unlock();

        wait();
    }

    void prepareRenderList();
    void setNewSize(int s);
    void prepareBuffer();
    void Prepare();
    void Abort() {
        m_abort = true;
        GMessages::Message("Aborting threaded rendering!");

    }
    Galaxy* AddGalaxy(QString file, QVector3D position, QVector3D orientation, float iscale, float redshift, QString name);

    Galaxy* AddGalaxy(GalaxyInstance* gi);

    void RenderPixels();
    void AssembleImage();
    void Render();
    void RenderDirect();
    void InitializeRendering();
    QVector3D setupCamera(int idx);
    RasterPixel* renderPixel(QVector3D dir, QVector<GalaxyInstance*> gals);
    void getIntensity(GalaxyInstance* gi, RasterPixel* rp, QVector3D isp1, QVector3D isp2);




		
    QImage *getBuffer() const;
    RenderingParams* getRenderingParams();
    State getState();
    void setState(const State &state);
    float getPercentDone() const;
};


