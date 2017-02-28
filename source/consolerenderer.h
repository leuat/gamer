#pragma once

#include <QString>
#include <QStringList>
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/rasterizer.h"


class ConsoleRenderer
{
private:
    void PrintUsage();
    void RenderSingleGalaxy(QStringList param);
    void RenderSkybox(QStringList param);
    QVector3D fromList(QStringList* lst, int i1, int i2, int i3);


    RenderingParams m_renderingParams;
    Rasterizer* m_rasterizer;



public:
    ConsoleRenderer(int argc, char *argv[]);
    ~ConsoleRenderer();


};

