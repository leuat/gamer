#pragma once

#include <QVector>
#include <QImage>
#include "source/galaxy/galaxy.h"
#include "source/galaxy/renderingparams.h"
#include "source/galaxy/galaxyinstance.h"
#include "source/galaxy/rasterpixel.h"
#include "source/noise/simplexnoise.h"


class Rasterizer {

private:
    QVector<GalaxyInstance*> m_galaxies;

			// All spectra
	//		CSpectra spectra;
			// Rendering parameters. Duh.
    RenderingParams m_renderingParams;
    RenderingParams m_oldRenderingParams;
    QVector<int> m_renderList;
    QImage* m_buffer = nullptr ;


    //enum RenderState {Idle, Performing, Postprocessing, RequestCancel, Cleanup};
    //static RenderState currentState = RenderState.Idle;
		
    //List<Task> taskList = new List<Task>();
    //public static Task currentTask = null;
    //public GamerCamera resetCamera = null;

    //Stopwatch stopWatch = new Stopwatch();
    //public static bool lowlevel = true;

    //Material material;
    //ColorBuffer2D stars;
    //const int maxTimer = 1;
    //int timer = maxTimer;
    //ThreadRenderState[]  threadRenderStates;
    //public float time;

 public:

//    static Simp simplex = new Simplex();
    void prepareRenderList();
    void setNewSize(int s);
    void prepareBuffer();
    void Prepare();

    Galaxy* AddGalaxy(QString file, QVector3D position, QVector3D orientation, float iscale, float redshift, QString name);

    Galaxy* AddGalaxy(GalaxyInstance* gi);

    void RenderPixels();
    void AssembleImage();
    void Render();
    void InitializeRendering();
    QVector3D setupCamera(int idx);
    RasterPixel* renderPixel(QVector3D dir, QVector<GalaxyInstance*> gals);
    void getIntensity(GalaxyInstance* gi, RasterPixel* rp, QVector3D isp1, QVector3D isp2, QVector3D camera);




		
    QImage *getBuffer() const;
};


