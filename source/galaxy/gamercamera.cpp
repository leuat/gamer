#define _USE_MATH_DEFINES
#include <cmath>
#include <QMatrix3x3>
#include "source/galaxy/gamercamera.h"
#include "math.h"
#include <QVector2D>
#include <QOpenGLFunctions>

QVector3D& GamerCamera::camera()
{
    return m_camera;
}

void GamerCamera::setCamera(const QVector3D &camera)
{
    m_camera = camera;
}

QVector3D& GamerCamera::target()
{
    return m_target;
}

void GamerCamera::setTarget(const QVector3D &target)
{
    m_target = target;
}

QVector3D& GamerCamera::up()
{
    return m_up;
}

void GamerCamera::setUp(const QVector3D &up)
{
    m_up = up;
}

float GamerCamera::perspective() const
{
    return m_perspective;
}

void GamerCamera::setPerspective(float perspective)
{
    m_perspective = perspective;
}

QMatrix4x4& GamerCamera::rotMatrix()
{
    return m_rotMatrix;
}

void GamerCamera::setRotMatrix(const QMatrix4x4 &rotMatrix)
{
    m_rotMatrix = rotMatrix;
}

QMatrix4x4& GamerCamera::viewMatrix()
{
    return m_viewMatrix;
}

void GamerCamera::setViewMatrix(const QMatrix4x4 &viewMatrix)
{
    m_viewMatrix = viewMatrix;
}

void GamerCamera::setRotation(QVector3D& v) {
    QMatrix3x3 rot = QQuaternion::fromEulerAngles(v).toRotationMatrix();
    m_rotMatrix.setToIdentity();
    for (int i=0;i<3;i++)
        for (int j=0;j<3;j++)
            m_rotMatrix(i,j) = rot(i,j);
//    m_rotMatrix = m_rotMatrix*rot;
    //QMatrix4x4::TRS(Vector3.zero, Quaternion.EulerAngles(v), Vector3.one);
    setupViewmatrix();
}


GamerCamera* GamerCamera::copy() {
    GamerCamera* g = new GamerCamera();
    g->setCamera(m_camera);
    g->setTarget(m_target);
    g->setPerspective(m_perspective);
    g->setRotMatrix(m_rotMatrix);
    g->setUp(m_up);
    return g;
}


GamerCamera::GamerCamera() {
    m_rotMatrix.setToIdentity();
}

void GamerCamera::TranslateXY(QVector3D& delta) {
    QVector3D right = QVector3D::crossProduct((m_camera-m_target), m_up).normalized();
    QVector3D d =  -delta.y()*m_up.normalized() + right*delta.x();
    m_camera = m_camera + d;
    m_target = m_target + d;
}

void GamerCamera::ZoomXY(float delta) {
    m_camera = m_camera - (m_camera-m_target).normalized()*delta;
}

void GamerCamera::RotateUp(float r) {
    QVector3D d = (m_camera-m_target).normalized();
    QVector3D right = QVector3D::crossProduct(d, m_up).normalized();
    m_up = QVector3D::crossProduct(right, d).normalized();
    QQuaternion q = QQuaternion::fromAxisAndAngle(d, r);
    m_up = q*m_up;
}


/*void GamerCamera::RotateXY(QVector3D& delta) {
    //gamer.rast.RP.camera.RotateVertical(delta.x*10);
    // gamer.rast.RP.camera.RotateHorisontal(delta.y*10);

}
*/
QMatrix4x4 GamerCamera::GetRotationMatrix() {
/*    QVector3D zaxis = (m_camera-m_target).normalized();
    QVector3D xaxis = (QVector3D::crossProduct(m_up, zaxis)).normalized()*-1;
    QVector3D yaxis = (QVector3D::crossProduct(zaxis, xaxis)).normalized();

    QMatrix4x4 M;
    M.setToIdentity();
    M(0,0) = xaxis.x();
    M(1, 0) = yaxis.x();
    M(2, 0) = zaxis.x();

    M(0, 1) = xaxis.y();
    M(1, 1) = yaxis.y();
    M(2, 1) = zaxis.y();

    M(0, 2) = xaxis.z();
    M(1, 2) = yaxis.z();
    M(2, 2) = zaxis.z();

    M(0, 3) = 0;
    M(1, 3) = 0;
    M(2, 3) = 0;
*/
//    return M;
    QMatrix4x4 M;
    M.setToIdentity();
//    M.lookAt( (m_camera-m_target),QVector3D(0,0,0),m_up);
    M.lookAt( QVector3D(0,0,0),(m_target-m_camera),m_up);
    return M;
    M = m_viewMatrix;
//    M.translate(m_camera);

    qDebug() << M;


//    M = m_viewMatrix.inverted();
/*    M(3, 0) = 0;
    M(3, 1) = 0;
    M(3, 2) = 0;
*/
    return M;

}

void GamerCamera::RotateVertical(float angle) {
    QVector3D d = m_camera - m_target;
    QVector3D side = QVector3D::crossProduct( m_up, d);
    QQuaternion q = QQuaternion::fromAxisAndAngle(side,angle);
    m_camera = q*(m_camera-m_target) + m_target;

    d = m_camera - m_target;
    m_up = QVector3D::crossProduct(d, side).normalized();
}
void GamerCamera::RotateHorisontal(float angle) {
    QVector3D d = m_camera - m_target;
    QVector3D side = QVector3D::crossProduct(m_up, d).normalized();
    QQuaternion q = QQuaternion::fromAxisAndAngle(m_up, angle);
    m_camera = q * (m_camera - m_target) + m_target;
    d = m_camera - m_target;
    m_up = QVector3D::crossProduct(d, side).normalized();
}


void GamerCamera::setupViewmatrix() {


/*    glm::mat4 proj = glm::perspective(FoV, AspectRatio, Near, Far);
       glm::mat4 view = glm::lookAt(glm::vec3(0.0f), CameraDirection, CameraUpVector);

       glm::mat4 invVP = glm::inverse(proj * view);
       glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
       glm::vec4 worldPos = invVP * screenPos;
  */

    m_projection.setToIdentity();
    m_projection.perspective(m_perspective,1,1,100);
    m_viewMatrix.setToIdentity();
//    m_viewMatrix.lookAt(m_target, m_camera, m_up);
    m_viewMatrix.lookAt(m_rotMatrix* m_target, m_rotMatrix* m_camera, m_rotMatrix* m_up);
//    m_viewMatrix.lookAt(m_camera, m_target, m_up);
//    qDebug() << m_camera;
//    m_viewMatrix = m_rotMatrix* m_viewMatrix;
    // Pre-calculate mvp
//    m_invVP = (m_projection.inverted()*m_viewMatrix).inverted();
    m_invVP = (m_projection*m_viewMatrix).inverted();

}

QVector3D GamerCamera::coord2ray(float x, float y, float width) {
    double xx = x / (width  * 0.5) - 1.0;
    double yy = y / (width * 0.5) - 1.0;

    QVector4D screenPos = QVector4D(xx, -yy, 1.0, 1.0);
    QVector4D worldPos = m_invVP * screenPos;
    return worldPos.toVector3D().normalized();
}


/*QVector3D GamerCamera::coord2ray(float x, float y, float width) {

    float aspect_ratio = 1;
    float FOV = m_perspective / 360.0f * 2 * M_PI; // convert to radians
    float dx=tan(FOV*0.5f)*(x/(width/2.0)-1.0f)/aspect_ratio;
    float dy=tan(FOV*0.5f)*(1.0- y/(width/2.0));

    float f = 10;

    QVector3D Pfar = QVector3D(dx*f, dy*f, f);
    QVector3D res = m_viewMatrix*Pfar;
    res = res.normalized();

    return res;
}
*/
