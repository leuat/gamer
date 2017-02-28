#include <QRgb>
#include <QColor>
#include <QElapsedTimer>

#include "source/galaxy/rasterizer.h"
#include "source/util/util.h"
#include "source/galaxy/galaxycomponent.h"
#include "source/util/util.h"
#include "source/util/gmessages.h"
#include "thread"
#include <omp.h>

RenderingParams* Rasterizer::getRenderingParams()
{
    return m_renderingParams;
}

Rasterizer::State Rasterizer::getState()
{
    return m_state;
}

void Rasterizer::setState(const State &state)
{
    m_state = state;
}

float Rasterizer::getPercentDone() const
{
    return m_percentDone;
}

QImage *Rasterizer::getImageShadowBuffer() const
{
    return m_imageShadowBuffer;
}

QElapsedTimer Rasterizer::getTimer() const
{
    return m_timer;
}

Buffer2D *Rasterizer::getRenderBuffer() const
{
    return m_renderBuffer;
}

void Rasterizer::setRenderingParams(RenderingParams *renderingParams)
{
    m_renderingParams = renderingParams;
}

QVector<GalaxyInstance *> Rasterizer::getGalaxies() const
{
    return m_galaxies;
}

bool Rasterizer::getAbort() const
{
    return m_abort;
}

Buffer2D *Rasterizer::getBackBuffer() const
{
    return m_backBuffer;
}

QVector<int> Rasterizer::getRenderList() const
{
    return m_renderList;
}

void Rasterizer::run()
{
    m_mutex.lock();
    m_mutex.unlock();
    Prepare();

    m_state=State::rendering;
    m_timer.start();
//    qDebug() << m_renderingParams->camera().perspective();
//    qDebug() << m_renderingParams->camera().camera();
    RenderPixels();
    GMessages::Message("Rendering took " + Util::MilisecondToString(m_timer.elapsed()));
    m_state=State::done;

}

void Rasterizer::Clear()
{
    m_galaxies.clear();
}

void Rasterizer::ReleaseBuffers()
{
    if (m_imageBuffer)
        delete m_imageBuffer;
    if (m_renderBuffer)
        delete m_renderBuffer;
    if (m_backBuffer)
        delete m_backBuffer;
    if (m_imageShadowBuffer)
        delete m_imageShadowBuffer;
    if (m_starsBuffer)
        delete m_starsBuffer;

}


void Rasterizer::prepareRenderList() {
    m_renderList.resize(m_renderingParams->size() * m_renderingParams->size());
    for (int i = 0; i < m_renderingParams->size() * m_renderingParams->size(); i++)
        m_renderList[i] = i;

    // Shuffle array
    m_renderList = Util::ShuffleArray<int>(m_renderList);
}



void Rasterizer::setNewSize(int s)
{
    if (s == m_renderingParams->size())
        return;

    m_renderingParams->setSize(s);
    prepareBuffer();
}

/*    void SetupStars() {



            if (stars!=null && RPold.noStars == m_renderingParams->noStars && RPold.starStrength == m_renderingParams->starStrength && RPold.starSize == m_renderingParams->starSize && m_renderingParams->starSizeSpread == RPold.starSizeSpread)
                if (m_renderingParams->size == stars._width)
                    return;

            stars = new ColorBuffer2D(m_renderingParams->size, m_renderingParams->size);
            stars.RenderStars(m_renderingParams->noStars, m_renderingParams->starSize/100f, m_renderingParams->starSizeSpread/100f,m_renderingParams->starStrength);
            RPold.starSizeSpread = m_renderingParams->starSizeSpread;
            RPold.starSize = m_renderingParams->starSize;
            RPold.noStars = m_renderingParams->noStars;
            RPold.starStrength = m_renderingParams->starStrength;

        }
*/
void Rasterizer::prepareBuffer()
{
    // Do not changes buffers if rendering!

    if (m_state == State::rendering) {
        qDebug() << "RETURNING STILL RENDERING";
        return;
    }


    int size = m_renderingParams->size();
    if (m_imageBuffer == nullptr || m_imageBuffer->width() != size || m_imageShadowBuffer->width()!=size) {
        ReleaseBuffers();
        m_imageBuffer = new QImage(size,size,QImage::Format_ARGB32);
        m_renderBuffer = new Buffer2D(size);
        m_backBuffer = new Buffer2D(size);
        m_imageShadowBuffer = new QImage(size, size,QImage::Format_ARGB32);
        m_starsBuffer = new Buffer2D(size);

        RenderStars();

    }

    m_imageBuffer->fill(QColor(0,0,0));
    m_backBuffer->fill(QVector3D(0,0,0));
    m_renderBuffer->fill(QVector3D(0,0,0));
    m_imageShadowBuffer->fill(QColor(0,0,0));


}
bool galaxySort(GalaxyInstance* a, GalaxyInstance* b)
{
    return a->position().length() > b->position().length();
}

void Rasterizer::Prepare() {

    // Sort
    for (GalaxyInstance* gi : m_galaxies)
        gi->setPosition(gi->position() - m_renderingParams->camera().camera());

    qSort(m_galaxies.begin(), m_galaxies.end(),  galaxySort);

    for (GalaxyInstance* gi : m_galaxies) {
//        qDebug() << gi->position().length();
        gi->setPosition(gi->position() + m_renderingParams->camera().camera());
    }

    // Done sorting
    for (GalaxyInstance* gi : m_galaxies) {
        gi->GetGalaxy()->galaxyParams().setNoise(m_noise);
        gi->GetGalaxy()->SetupComponents(m_renderingParams);
    }

    prepareBuffer();
    prepareRenderList();

    m_renderingParams->camera().setupViewmatrix();
}

void Rasterizer::RenderStars()
{
    if (m_starsBuffer==nullptr)
        return;

    m_starsBuffer->RenderStars(m_renderingParams->noStars(),
                               m_renderingParams->starSize(),
                               m_renderingParams->starSizeSpread(),
                               m_renderingParams->starStrength());

}



Galaxy* Rasterizer::AddGalaxy(QString file, QVector3D position, QVector3D orientation, float iscale, float redshift, QString name) {
    Galaxy* g = new Galaxy();
    g->Load(file);
    if (g!=nullptr)
        m_galaxies.append( new GalaxyInstance(g, name, position, orientation, iscale, redshift)  );
    return g;
}

Galaxy* Rasterizer::AddGalaxy(GalaxyInstance* gi) {
    m_galaxies.append (gi);
    return gi->GetGalaxy();
}

void Rasterizer::CopyFrom(Rasterizer *from)
{
    m_galaxies.clear();
    for (GalaxyInstance* gi: from->getGalaxies())
        m_galaxies.append( gi->Clone());
}

/*		public static void SaveGalaxyList(string filename, List<GalaxyInstance> gi) {
            XmlSerializer serializer = new XmlSerializer(typeof(List<GalaxyInstance>));
            TextWriter textWriter = new StreamWriter(filename);
            serializer.Serialize(textWriter, gi);
            textWriter.Close();

        }*/
/*		public static List<GalaxyInstance> LoadGalaxyList(string filename) {
            XmlSerializer deserializer = new XmlSerializer(typeof(List<GalaxyInstance>));
         //   UnityEngine.Debug.Log("Loading galaxy list: " + filename);
            TextReader textReader = new StreamReader(filename);
            List<GalaxyInstance> g = (List<GalaxyInstance>)deserializer.Deserialize(textReader);
            textReader.Close();
            foreach (GalaxyInstance gi in g) {
                UnityEngine.Debug.Log(Settings.GalaxyDirectory + gi.galaxyName + ".xml");
                Galaxy gx = Galaxy.Load(Settings.GalaxyDirectory + gi.galaxyName + ".xml", gi.galaxyName);
                UnityEngine.Debug.Log(gx);
                if (gx==null)
                    break;
                gi.SetGalaxy(gx);
                gi.GetGalaxy().displayName = gi.galaxyName;
            }
            return g;
        }
    */





void Rasterizer::RenderPixels() {
    RenderPixelsOMP();
}

void Rasterizer::RenderPixelsOMP()
{
    int size = pow(m_renderingParams->size(),2);
    float delta = 1.0/(float)size;
    m_percentDone = 0;
    int boxSize = min(m_renderingParams->size()/60, 4);
    if (m_isPreview)
        boxSize = 1;

//    boxSize = 1;

#pragma omp parallel for
    for (int k=0;k<size;k++) {
        if (m_abort)
            continue;
        int idx = m_renderList[ k ];

        QVector3D dir = setupCamera(idx);
        QVector3D I = renderPixel(dir, m_galaxies);
        m_percentDone+=delta;

        int i = idx%(int)m_renderingParams->size();
        int j = (idx-i)/(int)m_renderingParams->size();

        m_renderBuffer->DrawBox(m_backBuffer, i,j, boxSize, I);

    }

    m_abort = false;

}

void Rasterizer::AssembleImage()
{
    Buffer2D temp;
    m_renderBuffer->CopyTo(&temp);

    if (!m_isPreview)
        temp.Add(m_starsBuffer);

    temp.ToColorBuffer(m_imageBuffer, m_imageShadowBuffer,
                                  m_renderingParams->exposure(),
                                  m_renderingParams->gamma(),
                                  m_renderingParams->saturation());
}



/*		public void GenerateGalaxyList(int N, float size, string[] galaxies) {
            for (int i=0;i<N;i++) {
                Vector3 p = new Vector3();
                p.x = (float)((Util.rnd.NextDouble()-0.5)*size);
                p.y = (float)((Util.rnd.NextDouble()-0.5)*size);
                p.z = (float)((Util.rnd.NextDouble()-0.5)*size);
                Vector4 orient = new Vector3();
                orient.x = (float)((Util.rnd.NextDouble()-0.5));
                orient.y = (float)((Util.rnd.NextDouble()-0.5));
                orient.z = (float)((Util.rnd.NextDouble()-0.5));
                orient = orient.normalized;
                string n = galaxies[Util.rnd.Next()%galaxies.Length];
                AddGalaxy(Settings.GalaxyDirectory + n  + ".xml", p, orient, 0.5f + (float)Util.rnd.NextDouble(), 1, n);

            }

        }
    */

void  Rasterizer::Render() {

    if (!isRunning()) {
        QString  size = QString::number(m_renderingParams->size());
        GMessages::Message("Starting OMP rendering of image with size " + size + "x" + size + " on " + QString::number(std::thread::hardware_concurrency()) + " cores.");
        m_isPreview = false;
        start(HighPriority);
    } else {
        m_restart = true;
        m_condition.wakeOne();
    }

}


void Rasterizer::RenderDirect()
{
    Prepare();
    m_isPreview = true;
    RenderPixels();
}


QVector3D Rasterizer::setupCamera(int idx) {
    int i = idx%(int)m_renderingParams->size();
    int j = (idx-i)/(int)m_renderingParams->size();
    return m_renderingParams->camera().coord2ray(i,j, m_renderingParams->size());
}

QVector3D Rasterizer::renderPixel(QVector3D dir, QVector<GalaxyInstance*> gals) {
    QVector3D isp1, isp2;
    RasterPixel rp;// = new RasterPixel();

    for (int i=0;i<gals.size();i++) {
        GalaxyInstance* gi = gals[i];
        Galaxy* g = gi->GetGalaxy();
        if (g==nullptr) {
            qDebug() << "Error! Could not find galaxy in Rasterizer::renderPixel!" ;
            exit(1);
        }

        double t1, t2;
        bool intersects = Util::IntersectSphere(m_renderingParams->camera().camera() - gi->position(), dir,

                                                g->galaxyParams().axis(), isp1, isp2, t1, t2);

        if (t2>0) {
            // Behind! do nothing, take care of it in getIntensity
//            qDebug() << "BEHIND";
            isp2 = m_renderingParams->camera().camera()- gi->position();// + m_renderingParams->direction*
        }

        if (t1>0 && t2>0)
            intersects = false;

        if (intersects)
            getIntensity(gi, &rp, isp1, isp2);

    }
    return rp.I()*0.01/m_renderingParams->rayStep();
/*    rp->I()*=.01/m_renderingParams->rayStep();

    QVector3D I = rp->I();
    delete rp;
    return rp->I();*/
}

/*
 * The heart of the rendering: iterates through a galaxy
 *
 * */
void Rasterizer::getIntensity(GalaxyInstance* gi, RasterPixel* rp, QVector3D isp1, QVector3D isp2) {
    QVector3D origin = isp1;

    double length = (isp1-isp2).length();
    QVector3D dir = (isp1-isp2).normalized();
    Galaxy* g = gi->GetGalaxy();
    double step = m_renderingParams->rayStep();
    double N = (int)(length/step);
//    N = 2;
    QVector3D p = origin;
    rp->scale = step;


    QVector3D camera = m_renderingParams->camera().camera() - gi->position();
    int cnt = 0;
    float avgStep = 0;
    float minRayStep = 0.0005;
//    minRayStep = 0.05;
    if (m_isPreview) {
        minRayStep = 0.01;
//        m_renderingParams->setRayStep(0.005);
    }
    rp->dir = dir;
    while(QVector3D::dotProduct(p-origin,(isp2-origin).normalized())<length + step)
    {
        float curStep = Util::clamp((p-camera).length()*m_renderingParams->rayStep(), minRayStep, 0.05);
//            curStep = m_renderingParams->rayStep();
        step = curStep;
        avgStep +=step;

        float modifier = 0;
//        if (1==0)
        rp->step = step;

        if (QVector3D::dotProduct(p-camera, dir)<0)
            step = step + QVector3D::dotProduct(p-camera, dir);


        for ( GalaxyComponent* gc : g->components()) {
            if (gc->getComponentParams().active()==1)
            {
                ///  GalaxyComponent* c = g->com
                // Only if directed forwards
                if (QVector3D::dotProduct(p-camera, dir)>0)
                    if (gc->getComponentParams().className()!="bulge") {
                        rp->radius = gc->getRadius(p, rp->P, rp->z, gi);
                        rp->z = gc->getHeightModulation(rp->z);
                        // BULGEN er problemet for faen. jaja.
//                       float hmod = m_renderingParams->rayStep() + (1-rp->z)*0.01;
                       //curStep = max(min(curStep, hmod), m_renderingParams->rayStep());
  //                     curStep += (1-rp->z)*0.01;
    //                   step = curStep;
                       modifier = max(modifier, gc->calculateIntensity( rp, p, gi, step*200));
                    }

                if (gc->getComponentParams().className()=="bulge") {
    //                rp->setI( rp->I() + QVector3D(1,1,1)*step*100);
                    modifier = max(gc->calculateIntensity( rp, p, gi, step*200.0), modifier);
                }
            }
        }

//        curStep+= 0.1*exp(-modifier*1.0);

        step = curStep;
        p=p-dir*step;
        cnt++;

        rp->setI( Util::floor(rp->I()));

     //   if (QVector3D::dotProduct(p-camera, dir)<0)
    //        break;

    }
//    rp->setI( rp->I()/avgStep*1);

    //    if (rand()%100==1)
     //   qDebug() << "average: " << cnt << "curstep: " << avgStep/(float)cnt << "  lengthj : " << length;



}

QImage *Rasterizer::getBuffer() const
{
    return m_imageBuffer;
}

