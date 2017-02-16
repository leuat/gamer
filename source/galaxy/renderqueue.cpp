#include "renderqueue.h"

RenderingParams& RenderQueueItem::renderingParams()
{
    return m_renderingParams;
}

void RenderQueueItem::setRenderingParams(RenderingParams renderingParams)
{
    m_renderingParams = renderingParams;
}

void RenderQueue::setRasterizer(Rasterizer *rasterizer)
{
    m_rasterizer = rasterizer;
}

RenderQueue::RenderQueue()
{
    
}

RenderQueue::~RenderQueue()
{
    for (RenderQueueItem* qi : m_queue)
        delete qi;
}

void RenderQueue::Add(RenderingParams rp, QString filename)
{
    m_queue.append(new RenderQueueItem(rp, filename));
}

void RenderQueue::Update()
{
    // Pick first
    if (m_current==nullptr && m_queue.size()!=0)
        m_current = m_queue[0];

    if (m_current == nullptr)
        return;

    if (m_rasterizer->getState()==Rasterizer::State::idle) {

    }


}

RenderQueueItem::RenderQueueItem(RenderingParams rp, QString filename)
{
    m_renderingParams = rp;
    m_filename = filename;
}

QString RenderQueueItem::filename() const
{
    return m_filename;
}
