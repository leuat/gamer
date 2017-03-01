#include "hpxrasterizer.h"
#include "source/util/util.h"

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
    if (m_imageBuffer == nullptr || m_imageBuffer->width() != size) {
        ReleaseBuffers();
        m_imageBuffer = new QImage(size,size,QImage::Format_ARGB32);
        m_renderBuffer = new Buffer2D(size);
        qDebug() << m_nside;
        m_map = new Healpix_Map<float>();//m_nside, RING);
        m_map->SetNside(m_nside, RING);
        qDebug() << "Done allocating!";
    }

    m_imageBuffer->fill(QColor(0,0,0));
    m_renderBuffer->fill(QVector3D(0,0,0));


}

void HPXRasterizer::ReleaseBuffers()
{
    if (m_imageBuffer)
        delete m_imageBuffer;
    if (m_renderBuffer)
        delete m_renderBuffer;
    if (m_map)
        delete m_map;
}

void HPXRasterizer::PrepareRenderList()
{
    int size = 12*pow(m_nside,2);
    m_renderList.resize(size);
    for (int i = 0; i < size; i++)
        m_renderList[i] = i;

    // Shuffle array
    m_renderList = Util::ShuffleArray<int>(m_renderList);

}

void HPXRasterizer::RenderPixels()
{
    int size = 12*pow(m_nside,2);
    float delta = 1.0/(float)size;
    m_percentDone = 0;

#pragma omp parallel for
    for (int k=0;k<size;k++) {
        if (m_abort)
            continue;
        int idx = m_renderList[ k ];

        QVector3D dir = setupCamera(idx);
        QVector3D I = renderPixel(dir, m_galaxies);
        m_percentDone+=delta;


        (*m_map)[idx] = 1/3.0*(I.x() + I.y() + I.z());
        //(*m_map)[idx] = idx/100.0;
        //m_renderBuffer->DrawBox(m_backBuffer, i,j, boxSize, I);

    }

    m_abort = false;


}

QVector3D HPXRasterizer::setupCamera(int idx)
{
/*    int i = idx%(int)m_renderingParams->size();
    int j = (idx-i)/(int)m_renderingParams->size();
    return m_renderingParams->camera().coord2ray(i,j, m_renderingParams->size());
  */
    pointing p = m_map->pix2ang(idx);
    return Util::fromSpherical(1, p.phi -M_PI/2, p.theta+ M_PI/2);
}

void HPXRasterizer::AssembleImage()
{
    m_renderBuffer->MollweideProjection(*m_map);


    m_renderBuffer->ToColorBuffer(m_imageBuffer,
                                  m_renderingParams->exposure(),
                                  m_renderingParams->gamma(),
                                  m_renderingParams->saturation());

}
