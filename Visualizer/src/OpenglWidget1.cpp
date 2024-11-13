#include "OpenGlWidget1.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>

OpenGlWidget1::OpenGlWidget1(QWidget* parent)
    : QOpenGLWidget(parent),
    vbo(QOpenGLBuffer::VertexBuffer),
    isInitialized(false)
{
}

OpenGlWidget1::~OpenGlWidget1() {
    makeCurrent();
    vbo.destroy();
    doneCurrent();
}

void OpenGlWidget1::setData(Data inData)
{
    data = inData;
    makeCurrent();
    initializeGL();
    update();
}


QSize OpenGlWidget1::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize OpenGlWidget1::sizeHint() const
{
    return QSize(800, 800);
}

void OpenGlWidget1::sync(float inZoomLevel, QVector3D inRotation, QVector2D inPanOffset)
{
    zoomLevel = inZoomLevel;
    rotation = inRotation;
    panOffset = inPanOffset;
    update();
}

void OpenGlWidget1::initializeGL()
{
    if (data.vertices.size() > 0 && data.normals.size() > 0)
    {
        initializeOpenGLFunctions();
        glEnable(GL_DEPTH_TEST);

        // Load shader program
        bool shaderLoaded = false;

        shaderLoaded = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vertex1.glsl");
        shaderLoaded = shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fragment1.glsl");
        shaderProgram.link();

        // Load STL data
        //loadSTL("path/to/your/model.stl");

        // Prepare VBO
        std::vector<float> vertexData;
        for (int i = 0; i < data.vertices.size(); i = i + 3)
        {
            vertexData.push_back(data.vertices[i]);
            vertexData.push_back(data.vertices[i + 1]);
            vertexData.push_back(data.vertices[i + 2]);
            vertexData.push_back(data.normals[i]);
            vertexData.push_back(data.normals[i + 1]);
            vertexData.push_back(data.normals[i + 2]);
        }

        vbo.create();
        vbo.bind();
        vbo.allocate(vertexData.data(), static_cast<int>(vertexData.size() * sizeof(float)));

        shaderProgram.bind();
        shaderProgram.enableAttributeArray(0); // Position attribute
        shaderProgram.enableAttributeArray(1); // Normal attribute
        shaderProgram.setAttributeBuffer(0, GL_FLOAT, 0, 3, 6 * sizeof(float));
        shaderProgram.setAttributeBuffer(1, GL_FLOAT, 3 * sizeof(float), 3, 6 * sizeof(float));
        shaderProgram.release();
        isInitialized = true;
    }
}




void OpenGlWidget1::resizeGL(int w, int h) {
    if (isInitialized == true)
    {
        glViewport(0, 0, w, h);
    }
    projection.setToIdentity();
    projection.perspective(45.0f, float(w) / float(h), 0.1f, 100.0f);
}

void OpenGlWidget1::paintGL()
{
    if (isInitialized == true)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        updateModelViewMatrix();

        shaderProgram.bind();
        shaderProgram.setUniformValue("projection", projection);
        shaderProgram.setUniformValue("modelView", modelView);

        QVector3D lightPos(0.5f, 0.5f, 1.0f);
        shaderProgram.setUniformValue("lightPos", lightPos);
        shaderProgram.setUniformValue("viewPos", QVector3D(0.0f, 0.0f, 5.0f));

        vbo.bind();
        glDrawArrays(GL_TRIANGLES, 0, data.vertices.size());
        vbo.release();

        shaderProgram.release();
    }
}


void OpenGlWidget1::updateModelViewMatrix() {
    modelView.setToIdentity();
    modelView.translate(panOffset.x(), panOffset.y(), -5.0f);
    modelView.scale(zoomLevel);
    modelView.rotate(rotation.x(), 1.0f, 0.0f, 0.0f);
    modelView.rotate(rotation.y(), 0.0f, 1.0f, 0.0f);
    modelView.rotate(rotation.z(), 0.0f, 0.0f, 1.0f);
}

void OpenGlWidget1::wheelEvent(QWheelEvent* event) {
    // Zoom in or out
    if (event->angleDelta().y() > 0)
        zoomLevel *= 1.1f;  // Zoom in
    else
        zoomLevel /= 1.1f;  // Zoom out
    update();

    viewChange(zoomLevel, rotation, panOffset);

}

void OpenGlWidget1::mousePressEvent(QMouseEvent* event) {
    lastMousePosition = event->pos();
}

void OpenGlWidget1::mouseMoveEvent(QMouseEvent* event) {
    QPoint delta = event->pos() - lastMousePosition;
    lastMousePosition = event->pos();

    if (event->buttons() & Qt::LeftButton) {
        // Rotate model
        rotation.setX(rotation.x() + delta.y() * 0.5f);
        rotation.setY(rotation.y() + delta.x() * 0.5f);
    }
    else if (event->buttons() & Qt::RightButton) {
        // Pan model
        panOffset.setX(panOffset.x() + delta.x() * 0.01f);
        panOffset.setY(panOffset.y() - delta.y() * 0.01f);
    }
    update();

    viewChange(zoomLevel, rotation, panOffset);
}
