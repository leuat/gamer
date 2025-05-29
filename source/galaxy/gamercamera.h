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

    double perspective() const;
    void setPerspective(double perspective);

    QMatrix4x4& rotMatrix();
    void setRotMatrix(const QMatrix4x4 &rotMatrix);

    QVector3D m_camera = QVector3D(3,3,3.0);
    QVector3D m_target = QVector3D(0,0,0);
    QVector3D m_up = QVector3D(0,1,0);
    double m_perspective = 70;


    friend QDataStream& operator << ( QDataStream & s, GamerCamera& gc) {
        s << gc.m_camera << gc.m_target << gc.m_up << gc.m_perspective;
        return s;
    }

    friend QDataStream& operator >> ( QDataStream & s, GamerCamera& gc) {
        s >> gc.m_camera >> gc.m_target >> gc.m_up >> gc.m_perspective;
        return s;
    }

private:

    QMatrix4x4 m_viewMatrix, m_viewMatrixInv;
    QMatrix4x4 m_rotMatrix, m_projection, m_invVP;

    void setRotation(QVector3D& v);


    GamerCamera* copy();
public:

    GamerCamera();

    void TranslateXY(QVector3D& delta);

    void ZoomXY(double delta);

    void RotateUp(double r);

    QMatrix4x4 GetRotationMatrix();
    void setupViewmatrix();

    void RotateVertical(double angle);
    void RotateHorisontal(double angle);

    QVector3D coord2ray(double x, double y, double width);


    QMatrix4x4& viewMatrix();
    void setViewMatrix(const QMatrix4x4 &viewMatrix);
};

