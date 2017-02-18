#include "renderqueue.h"
#include "source/util/fitsio.h"
#include <QDebug>

RenderingParams& RenderQueueItem::renderingParams()
{
    return m_renderingParams;
}

void RenderQueueItem::setRenderingParams(RenderingParams renderingParams)
{
    m_renderingParams = renderingParams;
}

RenderQueueItem *RenderQueue::current() const
{
    return m_current;
}

RenderQueue::RenderQueue()
{

}

RenderQueue::~RenderQueue()
{
    for (RenderQueueItem* qi : m_queue)
        delete qi;
}

void RenderQueue::Add(Rasterizer* r, RenderingParams rp, QString filename)
{
    m_queue.append(new RenderQueueItem(r, rp, filename));
}



void RenderQueue::PostRendering() {

    m_current->rasterizer().AssembleImage();
    QString baseFile = m_current->filename();
    QString filename = m_current->renderingParams().imageDirectory() +
            baseFile +".png";

/*    if (ui->chkSaveFits->isChecked()) {
        FitsIO::SaveFloat(m_renderingParams.imageDirectory() + baseFile + "_red.fits", 0, m_rasterizer->getRenderBuffer());
        FitsIO::SaveFloat(m_renderingParams.imageDirectory() + baseFile + "_green.fits", 1, m_rasterizer->getRenderBuffer());
        FitsIO::SaveFloat(m_renderingParams.imageDirectory() + baseFile + "_blue.fits", 2, m_rasterizer->getRenderBuffer());
    }
//      FitsIO::SaveFloat(m_renderingParams.imageDirectory() +"test.fits", 0, m_rasterizer->getRenderBuffer());
*/


//    m_rasterizer.getImageShadowBuffer()->save(filename);
    m_current->rasterizer().getImageShadowBuffer()->save(filename);
    GMessages::Message("Galaxy png saved to " + filename);

}



void RenderQueue::Update()
{
    // Pick first
    if (m_current==nullptr && m_queue.size()!=0)
        m_current = m_queue[0];

    if (m_current == nullptr)
        return;

    if (m_current->rasterizer().getState() == Rasterizer::State::idle) {
        m_current->rasterizer().setRenderingParams(&m_current->renderingParams());
        m_current->rasterizer().Render();

    }
    if (m_current->rasterizer().getState() == Rasterizer::State::done) {
        PostRendering();
        m_queue.removeAll(m_current);
        m_current = nullptr;
    }


}

bool RenderQueue::isRendering()
{
    if (m_current==nullptr)
        return false;
    return true;
}

void RenderQueue::Abort()
{
    if (m_current!=nullptr)
        m_current->rasterizer().Abort();

    m_queue.clear();
    m_current = nullptr;

}

Rasterizer& RenderQueueItem::rasterizer()
{
    return m_rasterizer;
}

RenderQueueItem::RenderQueueItem(Rasterizer* r, RenderingParams rp, QString filename)
{
//    m_rasterizer = r;
    m_renderingParams = rp;
    m_rasterizer.setRenderingParams(&m_renderingParams);
    m_rasterizer.CopyFrom(r);
    m_filename = filename;
}

QString RenderQueueItem::filename() const
{
    return m_filename;
}
