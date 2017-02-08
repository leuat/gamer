#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QQuaternion>

class GamerCamera {
public:
    QVector3D& camera();
    void setCamera(const QVector3D &camera);

    QVector3D& target();
    void setTarget(const QVector3D &target);

    QVector3D& up();
    void setUp(const QVector3D &up);

    float perspective() const;
    void setPerspective(float perspective);

    QMatrix4x4& rotMatrix();
    void setRotMatrix(const QMatrix4x4 &rotMatrix);

private:
    QVector3D m_camera = QVector3D(3,3,3.0);
    QVector3D m_target = QVector3D(0,0,0);
    QVector3D m_up = QVector3D(0,1,0);
    float m_perspective = 70;

    QMatrix4x4 m_viewMatrix, m_viewMatrixInv;
    QMatrix4x4 m_rotMatrix;

    void setRotation(QVector3D& v);


    GamerCamera* copy();
public:

    GamerCamera();

    void TranslateXY(QVector3D& delta);

    void ZoomXY(QVector3D& delta);

    void RotateUp(float r);


    void RotateXY(QVector3D& delta) {
        //gamer.rast.RP.camera.RotateVertical(delta.x*10);
        // gamer.rast.RP.camera.RotateHorisontal(delta.y*10);

    }

    QMatrix4x4 GetRotationMatrix();
    void setupViewmatrix();

    void RotateVertical(float angle);
    void RotateHorisontal(float angle);

    QVector3D coord2ray(float x, float y, float width, float height);


    QMatrix4x4& viewMatrix();
    void setViewMatrix(const QMatrix4x4 &viewMatrix);
};

