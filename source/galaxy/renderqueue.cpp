#include "renderqueue.h"
#include "source/util/fitsio.h"
#include "source/util/util.h"
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
        FitsIO::Savedouble(m_renderingParams.imageDirectory() + baseFile + "_red.fits", 0, m_rasterizer->getRenderBuffer());
        FitsIO::Savedouble(m_renderingParams.imageDirectory() + baseFile + "_green.fits", 1, m_rasterizer->getRenderBuffer());
        FitsIO::Savedouble(m_renderingParams.imageDirectory() + baseFile + "_blue.fits", 2, m_rasterizer->getRenderBuffer());
    }
//      FitsIO::Savedouble(m_renderingParams.imageDirectory() +"test.fits", 0, m_rasterizer->getRenderBuffer());
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
        delete m_current;
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

    for (RenderQueueItem* rq : m_queue)
        delete rq;
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



void RenderQueue::RenderSkybox(Rasterizer* rasterizer, RenderingParams renderingParams) {

    QVector<QVector3D> planes, ups;
    planes.resize(6);
    ups.resize(6);
    planes[0] = QVector3D(0,0,-1);
    planes[1] = QVector3D(0,0,1);
    planes[2] = QVector3D(0,1,0);
    planes[3] = QVector3D(0,-1,0);
    planes[4] = QVector3D(1,0,0);
    planes[5] = QVector3D(-1,0,0);

    ups[0] = QVector3D(0,1,0);
    ups[1] = QVector3D(0,1,0);
    ups[2] = QVector3D(0,0,-1);
    ups[3] = QVector3D(0,0,1);
    ups[4] = QVector3D(0,1,0);
    ups[5] = QVector3D(0,1,0);

    QVector<QString> names;
    names.resize(6);
    names[0] ="Z-";
    names[1] ="Z+";
    names[2] ="Y-";
    names[3] ="Y+";
    names[4] ="X-";
    names[5] ="X+";

    //m_renderingParams.camera().setRotMatrix();
    RenderingParams reset = renderingParams;
    //RP.camera.setRotMatrix(resetCamera.GetRotationMatrix());

    for (int i=0;i<6;i++) {
        renderingParams.camera().setRotMatrix(reset.camera().GetRotationMatrix()  );
        renderingParams.camera().setCamera( reset.camera().camera() );
        renderingParams.camera().setTarget( reset.camera().camera() + planes[i] );
        renderingParams.camera().setUp(ups[i]);
        renderingParams.camera().setPerspective(90);
        //QString fname = Util::getFileName(m_renderingParams.imageDirectory(),"SkyboxZ-","png");
        QString fname = "Skybox" + names[i];
        Add(rasterizer, renderingParams, fname);
    }


}

