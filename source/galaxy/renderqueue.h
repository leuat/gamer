#pragma once

#include <QString>
#include <QVector>
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/rasterizer.h"

class RenderQueueItem
{
private:
    Rasterizer m_rasterizer;
    RenderingParams m_renderingParams;
    QString m_filename;

public:
    RenderQueueItem(Rasterizer* r, RenderingParams rp, QString filename);
    RenderingParams& renderingParams();
    void setRenderingParams(RenderingParams renderingParams);
    QString filename() const;
    Rasterizer& rasterizer() ;
};

class RenderQueue {
private:
    QVector<RenderQueueItem*> m_queue;
    RenderQueueItem* m_current = nullptr;

public:
    RenderQueue();
    ~RenderQueue();

    void Add(Rasterizer* r, RenderingParams rp, QString filename);
    void PostRendering();
    void Update();
    bool isRendering();
    void Abort();
    void RenderSkybox(Rasterizer* rasterizer, RenderingParams renderingParams);

    RenderQueueItem *current() const;
};
