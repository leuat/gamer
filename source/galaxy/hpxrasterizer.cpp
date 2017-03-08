#include "hpxrasterizer.h"
#include "source/util/util.h"

bool HPXRasterizer::onlyDust() const
{
    return m_onlyDust;
}

void HPXRasterizer::setOnlyDust(bool onlyDust)
{
    m_onlyDust = onlyDust;
}

HPXRasterizer::HPXRasterizer()
{

}

void HPXRasterizer::PrepareBuffer()
{
    if (m_state == State::rendering) {
        qDebug() << "RETURNING STILL RENDERING";
        return;
    }


    int size = m_renderingParams->size();
    if (m_imageBuffer == nullptr || m_imageBuffer->width() != size || m_map==nullptr ||
            m_map->Nside()  != m_renderingParams->nside() ) {
        ReleaseBuffers();
        m_imageBuffer = new QImage(size,size,QImage::Format_ARGB32);
        m_renderBuffer = new Buffer2D(size);
        m_map = new Healpix_Map<float>();//m_nside, RING);
        m_map->SetNside(m_renderingParams->nside(), RING);
    }
    m_imageBuffer->fill(QColor(0,0,0));
    m_renderBuffer->fill(QVector3D(0,0,0));


}

void HPXRasterizer::ReleaseBuffers()
{
    m_imageShadowBuffer = nullptr;
    if (m_imageBuffer)
        delete m_imageBuffer;
    if (m_renderBuffer)
        delete m_renderBuffer;
    if (m_map)
        delete m_map;


    m_imageBuffer = nullptr;
    m_renderBuffer = nullptr;
    m_map = nullptr;
}

void HPXRasterizer::PrepareRenderList()
{
    int size = 12*pow(m_renderingParams->nside(),2);
    m_renderList.resize(size);
    for (int i = 0; i < size; i++)
        m_renderList[i] = i;

    // Shuffle array
    m_renderList = Util::ShuffleArray<int>(m_renderList);

}

void HPXRasterizer::RenderPixels()
{
    int size = 12*pow(m_renderingParams->nside(),2);
    float delta = 1.0/(float)size;
    m_percentDone = 0;

    m_rotMatrix = QQuaternion::fromEulerAngles(QVector3D(90,0,0));

#pragma omp parallel for
    for (int k=0;k<size;k++) {
        if (m_abort)
            continue;
        int idx = m_renderList[ k ];

        QVector3D dir = m_rotMatrix*setupCamera(idx);
//        dir = m_rotMatrix*dir;
/*        float tmp = dir.x();
        dir.setX(dir.y());
        dir.setY(tmp);*/
        RasterPixel rp = renderPixel(dir, m_galaxies);
        m_percentDone+=delta;

        float sum = 1/3.0*(rp.I().x() + rp.I().y() + rp.I().z());
//        if (!m_onlyDust)
          (*m_map)[idx] = sum;
  /*      else {
            if (rp.dust.length()>0)
            (*m_map)[idx] = 10-sum;
            //else (*m_map)[idx] = 0;
        }*/
        //(*m_map)[idx] = idx/100.0;
        //m_renderBuffer->DrawBox(m_backBuffer, i,j, boxSize, I);

    }

    m_abort = false;


}

QVector3D HPXRasterizer::setupCamera(int idx)
{
    vec3 p = m_map->pix2vec(idx);
    return QVector3D(p.x, p.y, p.z);
}

void HPXRasterizer::AssembleImage()
{
    m_renderBuffer->MollweideProjection(*m_map);


    m_renderBuffer->ToColorBuffer(m_imageBuffer,
                                  m_renderingParams->exposure(),
                                  m_renderingParams->gamma(),
                                  m_renderingParams->saturation());

    m_imageShadowBuffer = m_imageBuffer;
}
