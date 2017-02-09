#include "glwidget.h"
#include <QDebug>
#include <QTimer>
#include <QApplication>

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent) {}

GLWidget::~GLWidget(){ }

void GLWidget::initializeGL() {
    QSize viewport_size = size();
    initializeOpenGLFunctions();
    glViewport(0, 0, viewport_size.width(), viewport_size.height());

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-1, 1, -1, 1, 5, 700); // near and far match your triangle Z distance

    glMatrixMode(GL_MODELVIEW);
    glGenTextures(1, &m_textureID); // Obtain an id for the texture
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0,0,-5.0);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glColor3f(1,1,1);
    glVertex3f( -1.0f, 1.0f, 0.0f);

    glTexCoord2f(1,0);
    glColor3f(1,1,1);
    glVertex3f( 1.0f, 1.0f, 0.0f);

    glTexCoord2f(1,1);
    glColor3f(1,1,1);
    glVertex3f( 1.0f,-1.0f, 0.0f);

    glTexCoord2f(0,1);
    glColor3f(1,1,1);
    glVertex3f( -1.0f,-1.0f, 0.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    update();
}

void GLWidget::resizeGL(int w, int h)
{
    QOpenGLWidget::resize(w,h);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    float strength = 0.5;
    if (event->buttons() & Qt::LeftButton) {
        m_RenderingParams->camera().RotateVertical(strength*dx);
        m_RenderingParams->camera().RotateHorisontal(strength*dy);
        m_redraw = true;
    }
    if(QApplication::keyboardModifiers().testFlag(Qt::AltModifier)) {

        m_RenderingParams->camera().ZoomXY(0.2*strength*dy);
        m_redraw = true;
    }
    m_lastPos = event->pos();
}

void GLWidget::loop()
{
    update();
}

bool GLWidget::redraw()
{
    if (m_redraw) {
        m_redraw = false;
        return true;
    }
    return false;
}

void GLWidget::SetTexture(QImage *image)
{
    glEnable(GL_TEXTURE_2D); // Enable texturing

    glBindTexture(GL_TEXTURE_2D, m_textureID); // Set as the current texture

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);


    int size = image->width();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->bits());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glDisable(GL_TEXTURE_2D);

    paintGL();
}
