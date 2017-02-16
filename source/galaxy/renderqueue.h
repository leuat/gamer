#pragma once

#include <QString>
#include <QVector>
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/rasterizer.h"

class RenderQueueItem
{
private:
    RenderingParams m_renderingParams;
    QString m_filename;

public:
    RenderQueueItem(RenderingParams rp, QString filename);
    RenderingParams& renderingParams();
    void setRenderingParams(RenderingParams renderingParams);
    QString filename() const;
};

class RenderQueue {
private:
    QVector<RenderQueueItem*> m_queue;
    Rasterizer* m_rasterizer;
    RenderQueueItem* m_current = nullptr;

public:
    RenderQueue();
    ~RenderQueue();

    void Add(RenderingParams rp, QString filename);
    void Update();

    void setRasterizer(Rasterizer *rasterizer);
};
