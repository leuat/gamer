#include <QRgb>
#include <QColor>
#include <QElapsedTimer>

#include "source/galaxy/rasterizer.h"
#include "source/util/util.h"
#include "source/galaxy/galaxycomponent.h"
#include "source/util/util.h"
#include "source/util/gmessages.h"
#include "thread"

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

void Rasterizer::run()
{
    m_mutex.lock();
    m_mutex.unlock();
    Prepare();
    m_state=State::rendering;

//    m_timer;
//    m_timer;
    m_timer.start();
//    sleep(5000);
    RenderPixels();
    GMessages::Message("Rendering took " + Util::MilisecondToString(m_timer.elapsed()));
    m_state=State::done;

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

    if (m_state == State::rendering)
        return;

    int size = m_renderingParams->size();

    if (m_imageBuffer == nullptr || m_imageBuffer->width() != size || m_imageShadowBuffer->width()!=size) {
        if (m_imageBuffer)
            delete m_imageBuffer;
        if (m_renderBuffer)
            delete m_renderBuffer;
        if (m_backBuffer)
            delete m_backBuffer;
        if (m_imageShadowBuffer)
            delete m_imageShadowBuffer;

        m_imageBuffer = new QImage(size,size,QImage::Format_ARGB32);
        m_renderBuffer = new Buffer2D(size);
        m_backBuffer = new Buffer2D(size);
        m_imageShadowBuffer = new QImage(size, size,QImage::Format_ARGB32);
    }
    m_imageBuffer->fill(QColor(0,0,0));
    m_backBuffer->fill(QVector3D(0,0,0));
    m_renderBuffer->fill(QVector3D(0,0,0));
    m_imageShadowBuffer->fill(QColor(0,0,0));

}


void Rasterizer::Prepare() {
    //sort (galaxies.begin(), galaxies.end(), galaxySort);

    /*			galaxies.Sort(
                delegate(GalaxyInstance i1, GalaxyInstance i2)
                {
                float d = (i1.position-m_renderingParams->camera.camera).magnitude - (i2.position-m_renderingParams->camera.camera).magnitude;
                if (d<0) return 1;
                else return -1;
//				return (i1.param.position-m_renderingParams->camera.camera).magnitude> (i2.param.position-m_renderingParams->camera.camera).magnitude;
                }
            );
            //            buffer = new ColorBuffer2D(m_renderingParams->size, m_renderingParams->size);
*/

    for (GalaxyInstance* gi : m_galaxies) {
        gi->GetGalaxy()->galaxyParams().setNoise(m_noise);
        gi->GetGalaxy()->SetupComponents();
    }

    prepareBuffer();
    prepareRenderList();

    m_renderingParams->camera().setupViewmatrix();
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

    int size = pow(m_renderingParams->size(),2);
    float delta = 1.0/(float)size;
    m_percentDone = 0;
#pragma omp parallel for
    for (int k=0;k<size;k++) {
        if (m_abort)
            continue;

        int idx = m_renderList[ k ];
        QVector3D dir = setupCamera(idx);
        RasterPixel rp = renderPixel(dir, m_galaxies);
        m_percentDone+=delta;

        int i = idx%(int)m_renderingParams->size();
        int j = (idx-i)/(int)m_renderingParams->size();



        //m_renderBuffer->setPixel(i,j,rgb.rgba());
        m_renderBuffer->DrawBox(m_backBuffer, i,j, 6, rp.I());

    }
    m_abort = false;
}

void Rasterizer::AssembleImage()
{
    m_renderBuffer->ToColorBuffer(m_imageBuffer, m_imageShadowBuffer,
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
        GMessages::Message("Starting rendering of image with size " + size + "x" + size + " on " + QString::number(std::thread::hardware_concurrency()) + " cores.");
        start(HighPriority);
    } else {
        m_restart = true;
        m_condition.wakeOne();
    }

}

void Rasterizer::RenderDirect()
{
    Prepare();
    RenderPixels();
}

/*
    void RenderSkybox() {

            Vector3[] planes = new Vector3[6];
            Vector3[] ups = new Vector3[6];
            planes[0] = new Vector3(0,0,-1);
            planes[1] = new Vector3(0,0,1);
            planes[2] = new Vector3(0,1,0);
            planes[3] = new Vector3(0,-1,0);
            planes[4] = new Vector3(1,0,0);
            planes[5] = new Vector3(-1,0,0);

            ups[0] = new Vector3(0,-1f,0);
            ups[1] = new Vector3(0,-1f,0);
            ups[2] = new Vector3(0,0,1);
            ups[3] = new Vector3(0,0,-1);
            ups[4] = new Vector3(0,-1f,0);
            ups[5] = new Vector3(0,-1f,0);

            string[] names = new string[6];
            names[0] ="Z-";
            names[1] ="Z+";
            names[2] ="Y+";
            names[3] ="Y-";
            names[4] ="X-";
            names[5] ="X+";

            resetCamera = m_renderingParams->camera.copy();
            
            time = 0;
            currentState = RenderState.RequestCancel;
            //			currentTask = null;
            Abort(); // threads
            //m_renderingParams->camera.setRotation(new Vector3(0, 45, 45));
            m_renderingParams->camera.setRotMatrix(resetCamera.GetRotationMatrix());
            //UnityEngine.Debug.Log(m_renderingParams->camera.rotMatrix);
            

            for (int i=0;i<6;i++) {
                RenderImageSaveTask r = new RenderImageSaveTask("skybox" + names[i] + ".png", this);
                
                r.camera = resetCamera.camera;
                r.target = resetCamera.camera + planes[i];
                r.up =  ups[i];

                r.FOV = 90;
                taskList.Add (r);
            }

            //			m_renderingParams->camera = oldCam;


            currentState = RenderState.Idle;
        }

        */

QVector3D Rasterizer::setupCamera(int idx) {
    int i = idx%(int)m_renderingParams->size();
    int j = (idx-i)/(int)m_renderingParams->size();


    return m_renderingParams->camera().coord2ray(i,j, m_renderingParams->size());
}

RasterPixel* Rasterizer::renderPixel(QVector3D dir, QVector<GalaxyInstance*> gals) {
    QVector3D isp1, isp2;
    //dir*=-1;
    RasterPixel* rp = new RasterPixel();

    QVector3D viewDir = (m_renderingParams->camera().camera()-m_renderingParams->camera().target()).normalized();
    if (QVector3D::dotProduct(viewDir,dir)<0)
        dir*=-1;

    for (int i=0;i<gals.size();i++) {
        GalaxyInstance* gi = gals[i];

        Galaxy* g = gi->GetGalaxy();
        float t1, t2;
        bool intersects = Util::IntersectSphere(m_renderingParams->camera().camera() - gi->position(), dir,

                                                g->galaxyParams().axis(), isp1, isp2, t1, t2);

        if (t1<0) {
            //isp2 = m_renderingParams->camera().camera()- gi->position();// + m_renderingParams->direction*
        }

        if (t1>0 && t2>0)
            intersects = false;

/*          if (random()%300==1)
                qDebug() << "average: " << "isps: " << isp1 << ", " << isp2;
*/

        if (intersects)
            getIntensity(gi, rp, isp1, isp2);

    }
    rp->I()*=.01/m_renderingParams->rayStep();

    return rp;
}


void Rasterizer::getIntensity(GalaxyInstance* gi, RasterPixel* rp, QVector3D isp1, QVector3D isp2) {
    QVector3D origin = isp1;
    float length = (isp1-isp2).length();
    QVector3D dir = (isp1-isp2).normalized();
    Galaxy* g = gi->GetGalaxy();
    float step = m_renderingParams->rayStep();
    int N = (int)(length/step);
    QVector3D p = origin;
    rp->scale = step;

    for (int i=0;i<N;i++)
//    while(QVector3D::dotProduct(p-origin,(isp2-origin).normalized())<length)
    {
        //step = Mathf.Clamp((p-camera).magnitude*0.01f, 0.001f, 0.01f);

        step = m_renderingParams->rayStep();
        for ( GalaxyComponent* gc : g->components()) {
            //            qDebug() << "Looping throgh gc " << gc->getComponentParams().className();
            if (gc->getComponentParams().active()==1)
                gc->calculateIntensity( rp, p, gi, step*200);
        }
        p=p-dir*step;
        //rp.I()Floor(0);
    }
/*    if (random()%300==1)
        qDebug() << "average: " << N << "isps: " << isp1 << ", " << isp2;*/
}

QImage *Rasterizer::getBuffer() const
{
    return m_imageBuffer;
}

