#pragma once

#include <QVector3D>
#include <QMatrix4x4>
#include <QMatrix3x3>
#include <QQuaternion>

class GamerCamera {
public:
    QVector3D camera() const;
    void setCamera(const QVector3D &camera);

    QVector3D target() const;
    void setTarget(const QVector3D &target);

    QVector3D up() const;
    void setUp(const QVector3D &up);

    float perspective() const;
    void setPerspective(float perspective);

    QMatrix4x4 rotMatrix() const;
    void setRotMatrix(const QMatrix4x4 &rotMatrix);

private:
    QVector3D m_camera, m_target, m_up;
    float m_perspective;

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


};

