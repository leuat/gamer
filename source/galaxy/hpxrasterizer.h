#pragma once

#include "source/galaxy/rasterizer.h"
#ifdef USE_HEALPIX
#include <healpix_map_fitsio.h>
#include <healpix_map.h>
#endif

class HPXRasterizer: public Rasterizer
{
#ifdef USE_HEALPIX
    Healpix_Map<float>* m_map = nullptr;//(0,RING);
    QQuaternion m_rotMatrix;
    bool m_onlyDust = true;
    int m_nside = 32;
    int m_nsidePreview = 32;
#endif

public:

    HPXRasterizer();
    HPXRasterizer(Rasterizer* rp) : Rasterizer(rp) { }
    HPXRasterizer(RenderingParams* rp) : Rasterizer(rp) { }

    void PrepareBuffer() override;
    void ReleaseBuffers() override;
    void PrepareRenderList() override;
    void RenderPixels() override;
    QVector3D setupCamera(int idx) override;
    void AssembleImage() override;
    bool onlyDust() const;
    void setOnlyDust(bool onlyDust);
};



