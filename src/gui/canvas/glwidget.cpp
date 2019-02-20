//
// Created by Gabriel on 2/8/2019.
//

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "transform3D.h"

bool GLWidget::m_transparent = false;

static const char *vertexShaderSourceCore = ":/shaders/logoShaderSourceCore.vert";

static const char *fragmentShaderSourceCore = ":/shaders/logoShaderSourceCore.frag";

static const char *vertexShaderSource = ":/shaders/logoShaderSource.vert";

static const char *fragmentShaderSource = ":/shaders/logoShaderSource.frag";

GLWidget::GLWidget(QWidget *parent)
        : QOpenGLWidget(parent),
          m_xRot(0),
          m_yRot(0),
          m_zRot(0),
          m_program(0)
{
    m_transform.translate(0.0f, 0.0f, -5.0f);
    m_core = QSurfaceFormat::defaultFormat().profile() == QSurfaceFormat::CoreProfile;
    // --transparent causes the clear color to be transparent. Therefore, on systems that
    // support it, the widget will become transparent apart from the logo.
    if (m_transparent) {
        QSurfaceFormat fmt = format();
        fmt.setAlphaBufferSize(8);
        setFormat(fmt);
    }
}

GLWidget::~GLWidget() {
    cleanup();
}

void GLWidget::cleanup() {
    if (m_program == nullptr)
        return;
    makeCurrent();
    m_objectVbo.destroy();
    delete m_program;
    m_program = 0;
    doneCurrent();
}

void GLWidget::initializeGL() {
    // In this example the widget's corresponding top-level window can change
    // several times during the widget's lifetime. Whenever this happens, the
    // QOpenGLWidget's associated context is destroyed and a new one is created.
    // Therefore we have to be prepared to clean up the resources on the
    // aboutToBeDestroyed() signal, instead of the destructor. The emission of
    // the signal will be followed by an invocation of initializeGL() where we
    // can recreate all resources.
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);
    //connect(this, SIGNAL(frameSwapped()), this, SLOT(update()));

    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, m_transparent ? 0 : 1);

    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex,  m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment,m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("normal", 1);
    m_program->bindAttributeLocation("color",  2);
    m_program->link();

    m_program->bind();
    // The name for the uniform locations come from the shader program
    u_worldToView = m_program->uniformLocation("worldToView");
    u_modelToWorld = m_program->uniformLocation("modelToWorld");
    m_normalMatrixLoc = m_program->uniformLocation("normalMatrix");
    m_lightPosLoc = m_program->uniformLocation("lightPos");

    // Create a vertex array object. In OpenGL ES 2.0 and OpenGL 2.x
    // implementations this is optional and support may not be present
    // at all. Nonetheless the below code works in all cases and makes
    // sure there is a VAO when one is needed.
    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    // Setup our vertex buffer object.
    m_objectVbo.create();
    m_objectVbo.bind();
    m_objectVbo.allocate(m_logo.constData(), m_logo.count() * sizeof(GLfloat));

    // Store the vertex attribute bindings for the program.
    setupVertexAttribs();

    // Our camera never changes in this example.
    m_camera.setToIdentity();
    m_camera.translate(0, 0, -1);

    // Light position is fixed.
    m_program->setUniformValue(m_lightPosLoc, QVector3D(0, 0, 70));

    m_program->release();
}

void GLWidget::setupVertexAttribs() {
    m_objectVbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);    // position
    f->glEnableVertexAttribArray(1);    // normal
    f->glEnableVertexAttribArray(2);    // color
    f->glVertexAttribPointer(0, Vertex::PositionTupleSize,GL_FLOAT, GL_FALSE, Vertex::stride(), 0);
    f->glVertexAttribPointer(1, Vertex::NormalTupleSize,  GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(3 * sizeof(GLfloat)));
    f->glVertexAttribPointer(2, Vertex::ColorTupleSize,   GL_FLOAT, GL_FALSE, Vertex::stride(), reinterpret_cast<void *>(6 * sizeof(GLfloat)));
    m_objectVbo.release();
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

//    m_world.setToIdentity();
//    m_world.rotate(180.0f - (m_xRot / 16.0f), 1, 0, 0);
//    m_world.rotate(m_yRot / 16.0f, 0, 1, 0);
//    m_world.rotate(m_zRot / 16.0f, 0, 0, 1);

    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();

//    m_program->setUniformValue(u_worldToView, m_projection);
//    {
//        //m_object.bind();
//        m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
//        glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());
//        //m_object.release();
//    }

    m_program->setUniformValue(u_worldToView, m_projection);
    //m_program->setUniformValue(u_modelToWorld, m_camera * m_world);
    m_program->setUniformValue(u_modelToWorld, m_transform.toMatrix());
    QMatrix3x3 normalMatrix = m_world.normalMatrix();
    m_program->setUniformValue(m_normalMatrixLoc, normalMatrix);
    glDrawArrays(GL_TRIANGLES, 0, m_logo.vertexCount());

    m_program->release();
}

void GLWidget::resizeGL(int w, int h) {
    m_projection.setToIdentity();
    m_projection.perspective(45.0f, GLfloat(w) / h, 0.01f, 100.0f);
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();
    Qt::MouseButtons mouseButtons = event->buttons();

    if(mouseButtons & Qt::LeftButton) {
        setXRotation(m_xRot + 8 * dy);
        setYRotation(m_yRot + 8 * dx);
        //setZRotation(m_zRot + 8 * dx);
        std::cout << m_xRot << std::endl;
    } else if(mouseButtons & Qt::RightButton) {
        float trans = m_transform.translation().z() + (dy * 0.005f);

        m_transform.translate(dx, dy, trans);
        qDebug() << m_transform;
        //update();
    } else if(mouseButtons & Qt::MidButton) {
        float scale = -std::min(m_transform.translation().z(), 0.0f) * 0.001f + 0.000025f;
        m_transform.scale(scale);

    }
    update();
    m_lastPos = event->pos();
}


QSize GLWidget::minimumSizeHint() const {
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const {
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle) {
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != m_xRot) {
        m_xRot = angle;
        m_transform.rotate(m_xRot, QVector3D(1.0f, 0.0f, 0.0f));
        emit xRotationChanged(angle);
        //update();
    }
}

void GLWidget::setYRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != m_yRot) {
        m_yRot = angle;
        m_transform.rotate(m_yRot, QVector3D(0.0f, 1.0f, 0.0f));
        emit yRotationChanged(angle);
        //update();
    }
}

void GLWidget::setZRotation(int angle) {
    qNormalizeAngle(angle);
    if (angle != m_zRot) {
        m_zRot = angle;
        m_transform.rotate(m_zRot, QVector3D(0.0f, 0.0f, 1.0f));
        emit zRotationChanged(angle);
        //update();
    }
}

void GLWidget::printVersionInformation() {
    QString glType;
    QString glVersion;
    QString glProfile;

    // Get Version Information
    glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
    glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

    // Get Profile Information
#define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
    switch (format().profile()) {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
    }
#undef CASE

    // qPrintable() will print our QString w/o quotes around it.
    qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";
}
