#pragma once

#include <QThread>
#include "source/galaxy/rasterizer.h"


class RasterThread : public Rasterizer
{
private:
    QVector<RasterThread*> m_threads;
    int m_from, m_to;
public:
    RasterThread(RenderingParams* rp) : Rasterizer(rp) { }
    RasterThread(Rasterizer* rp) : Rasterizer(rp) { }
    RasterThread(Rasterizer *rast, int from, int to)  {
        m_galaxies = rast->getGalaxies();
        m_renderingParams = rast->getRenderingParams();
        m_from = from;
        m_to = to;
        m_renderBuffer = rast->getRenderBuffer();
        m_renderList = rast->getRenderList();
        m_backBuffer = rast->getBackBuffer();
    }
    void Render() override;
    void RenderPixels() override;
    void AssembleImage() override;
    State getState() override;
    float getPercentDone() const override;
    void Abort() override;


protected:
    void run() override;

};

