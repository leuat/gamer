#include "rasterthread.h"
#include "rasterizer.h"
#include <thread>

void RasterThread::Render()
{
    m_threads.clear();
    int N = std::thread::hardware_concurrency();
    Prepare();
    int chunk = m_renderList.size()/N;
    int position = 0;
    m_timer.start();
    for (int i=0;i<N;i++) {
        RasterThread* rt = new RasterThread(this,position, position + chunk);
        position+=chunk;
        m_threads.append(rt);
        rt->start(HighPriority);
    }

}

void RasterThread::RenderPixels()
{
    if (m_isPreview) {
        RenderPixelsOMP();
        return;

    }
    int size = m_to - m_from;
    float delta = 1.0/(float)size;
    m_percentDone = 0;
    int boxSize = m_renderingParams->size()/60;
    for (int k=m_from;k<m_to;k++) {
        if (m_abort)
            continue;
//        if (m_isPreview)
//            qDebug() << k;
        int idx = m_renderList[ k ];

        QVector3D dir = setupCamera(idx);
        QVector3D I = renderPixel(dir, m_galaxies);
        m_percentDone+=delta;

        int i = idx%(int)m_renderingParams->size();
        int j = (idx-i)/(int)m_renderingParams->size();

        //m_renderBuffer->setPixel(i,j,rgb.rgba());
        m_renderBuffer->DrawBox(m_backBuffer, i,j, boxSize, I);
//        m_renderBuffer->DrawBox(m_backBuffer, i,j, 1, I);

    }

    m_abort = false;

}

void RasterThread::AssembleImage()
{
    Rasterizer::AssembleImage();

}

Rasterizer::State RasterThread::getState()
{
    if (m_threads.size() == 0)
        return m_state;

    for (Rasterizer* r : m_threads)
        if (r->getState()==State::rendering)
            return State::rendering;

    return State::done;

}

float RasterThread::getPercentDone() const
{
    if (m_threads.size()==0)
        return m_percentDone;

    float p = 0;
    for (RasterThread* rt : m_threads)
        p+=rt->getPercentDone();

    return p/(float)m_threads.size();

}

void RasterThread::Abort()
{
    if (m_threads.size()==0) {
        m_abort = true;
        requestInterruption();
        quit();
        wait();
        m_state = State::aborting;
    }
    else
        for (RasterThread* rt: m_threads)
            rt->Abort();

}


void RasterThread::run()
{
    m_state=State::rendering;
//    qDebug() << m_renderingParams->camera().perspective();
//    qDebug() << m_renderingParams->camera().camera();
    RenderPixels();
    m_state=State::done;

}
