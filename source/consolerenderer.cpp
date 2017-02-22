#include "consolerenderer.h"
#include <ostream>
#include "mainwindow.h"
#include "source/util/util.h"
#include "thread"
#include "source/galaxy/rasterthread.h"

using namespace std;

void ConsoleRenderer::PrintUsage()
{
    cout << "Usage: Gamer [ command ] [ parameters ]" << endl;
    cout << "Commands are : "<<endl;
    cout << "   - galaxy : renders a single galaxy with (example) parameters: " << endl;
    cout << "   [ method = omp/thread ] [ camera = 1 0 0  ] [ target = 0 0 0  ] [ up = 0 1 0 ] [ fov = 90 ]  [ exposure = 1 ] [ gamma = 1 ] [ saturation = 1 ] [ ray step = 0.025 ] [ galaxy gax file ] [ size = 256 ] [ filename = test{.png} ] " << endl;
    cout << endl;


}

void ConsoleRenderer::RenderSingleGalaxy(QStringList param)
{
    if (param.size()!=19) {
        qDebug() << param.size();
        cout << "Incorrect usage/parameters for galaxy. Usage: " << endl;
        PrintUsage();
        exit(1);

    }
    m_rasterizer = nullptr;
    if (param[1].toLower()=="omp") {
        m_rasterizer = new Rasterizer();
    }
    if (param[1].toLower()=="thread") {
        m_rasterizer = new RasterThread(&m_renderingParams);
    }
    if (m_rasterizer==nullptr) {
        cout << "ERROR! Cannot recognize " << param[1].toStdString() << endl;
        cout << "Must be 'omp' or 'thread'" << endl;
        exit(1);

    }

    m_renderingParams.camera().setCamera( fromList(&param,2,3,4));
    m_renderingParams.camera().setTarget( fromList(&param,5,6,7));
    m_renderingParams.camera().setUp( fromList(&param,8,9,10));
    m_renderingParams.camera().setPerspective( param[11].toFloat());
    m_renderingParams.setExposure( param[12].toFloat());
    m_renderingParams.setGamma( param[13].toFloat());
    m_renderingParams.setSaturation(param[14].toFloat());
    m_renderingParams.setRayStep(param[15].toFloat());
    QString galaxyFile = param[16];
    m_renderingParams.setSize(param[17].toFloat());
    QString outFile = param[18];

    Util::path = "";
    //m_renderingParams.Load("build/Gamer.app/RenderingParams.dat");
    m_renderingParams.spectra().PopulateSpectra();

    Galaxy galaxy;
//    cout << "Loading galaxy " << galaxyFile.toStdString() << endl;
    galaxy.Load(galaxyFile);

    m_rasterizer->setRenderingParams(&m_renderingParams);
    m_rasterizer->AddGalaxy(new GalaxyInstance(&galaxy, galaxy.galaxyParams().name(),
                                              QVector3D(0,0,0), QVector3D(0,1,0).normalized(), 1, 0)  );

    cout << "Starting rendering on " <<  QString::number(std::thread::hardware_concurrency()).toStdString() << " cores." << endl;
    Q_TIMER_START();
    m_rasterizer->Render();
    int prevP = 0;
    while (m_rasterizer->getState()!= Rasterizer::State::done) {
        int curP = (m_rasterizer->getPercentDone()*1000.0);
        if (curP!=prevP) {

            float time = ttimer.elapsed();
            float percentage = m_rasterizer->getPercentDone();
            float timeLeft = time/(percentage) - time;
            QString t;
            t.sprintf("%2.1f", curP*0.1);
            cout << "\r[ " << t.toStdString() <<  "% ]  with ETA in " << Util::MilisecondToString(timeLeft).toStdString() <<   std::flush;
            prevP = curP;

        }
    }
    cout << endl;

    Q_TIMER_ELAPSED("Rendering");
    m_rasterizer->AssembleImage();
    m_rasterizer->getImageShadowBuffer()->save(outFile + ".png");
    cout << "Image saved to file " << outFile.toStdString() << ".png" << endl;

}

QVector3D ConsoleRenderer::fromList(QStringList *lst, int i1, int i2, int i3)
{
    return QVector3D( (*lst)[i1].toFloat(),(*lst)[i2].toFloat(),(*lst)[i3].toFloat() );
}

ConsoleRenderer::ConsoleRenderer(int argc, char *argv[])
{
    cout << "Welcome to GAMER version " << MainWindow::Version() << endl;
    cout << endl;

    QStringList aLst;
    for (int i=1;i<argc;i++) {
        aLst.append(QString(argv[i]));
    }
    if (aLst[0].toLower() == "galaxy") {
        RenderSingleGalaxy(aLst);
        exit(0);
    }

    PrintUsage();

}

ConsoleRenderer::~ConsoleRenderer()
{

}
