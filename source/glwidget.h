#pragma once
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>
#include <QMouseEvent>
#include <QPoint>
#include "source/galaxy/renderingparams.h"

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    GLWidget(QWidget *parent = 0);
    void setRenderingParams(RenderingParams* rp) {
        m_RenderingParams = rp;
    }

   ~GLWidget();
    void SetTexture(QImage* image);
   //QSize sizeHint() const {  return QSize(400, 400); }

    bool redraw();
    void disableInput(bool value) {
        m_disableInput = value;
    }

    void setRedraw(bool redraw);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    GLuint m_textureID;
private slots:
    void loop();


private:
    bool inDrag;
    QPoint m_lastPos;
    RenderingParams* m_RenderingParams = nullptr;
    bool m_redraw = false;
    bool m_disableInput = false;
};
