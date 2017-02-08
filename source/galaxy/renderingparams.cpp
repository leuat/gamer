#include "source/galaxy/renderingparams.h"

int RenderingParams::size() const
{
    return m_size;
}

void RenderingParams::setSize(int size)
{
    m_size = size;
}

GamerCamera  &RenderingParams::camera()
{
    return m_camera;
}

void RenderingParams::setCamera(const GamerCamera &camera)
{
    m_camera = camera;
}

float RenderingParams::rayStep() const
{
    return m_rayStep;
}

void RenderingParams::setRayStep(float rayStep)
{
    m_rayStep = rayStep;
}
