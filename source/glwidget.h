#pragma once
#include <QGLWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QImage>

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    GLWidget(QWidget *parent = 0);
   ~GLWidget();
    void SetTexture(QImage* image);
   //QSize sizeHint() const {  return QSize(400, 400); }

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    GLuint m_textureID;
private slots:
    void loop();
};
