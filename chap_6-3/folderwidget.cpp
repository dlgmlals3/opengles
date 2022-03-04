#include "folderwidget.h"

FolderWidget::FolderWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      mIndexBuf(QOpenGLBuffer::IndexBuffer),
      mTexture(0),
      mWidth(0), mHeight(0), mAngle(0.0f),
      mNumOfVertices(0)
{
}

FolderWidget::~FolderWidget()
{
    makeCurrent();
    delete mTexture;
    doneCurrent();
}

void FolderWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0, 0, 0, 1);

    initShader();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mArrayBuf.create();
    mTexcoordBuf.create();
    mIndexBuf.create();

    mTexture = new QOpenGLTexture(QImage(":/res/images/car.jpg").mirrored());
    mTexture->setMinificationFilter(QOpenGLTexture::Linear);
    mTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    mTexture->setWrapMode(QOpenGLTexture::Repeat);

    mAnimation = new QVariantAnimation();
    mAnimation->setDuration(DURATION);
    mAnimation->setEasingCurve(QEasingCurve(QEasingCurve::Linear));
    mAnimation->setStartValue(QVariant(START_VALUE));
    mAnimation->setEndValue(QVariant(END_VALUE));
}

void FolderWidget::initShader() {
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/shaders/vertex.glsl")) {
        close();
    }
    if (!mProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/shaders/frag.glsl")) {
        close();
    }

    if (!mProgram.link()) {
        close();
    }
    if (!mProgram.bind()) {
        close();
    }
}

void FolderWidget::resizeGL(int w, int h) {
    mWidth = w;
    mHeight = h;
    const qreal zNear = w;
    const qreal zFar = w * 4.0;
    const qreal left = -(w / 2.0) / 2.0;
    const qreal right = -left;
    const qreal bottom = -(h / 2.0) / 2.0;
    const qreal top = -bottom;

    mProjection.setToIdentity();
    mProjection.frustum(left, right, bottom, top, zNear, zFar);

    createPlaneMesh(600, 400, 2, 2);
}

void FolderWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(0.0, 0.0, -2.0 * mWidth);

    mProgram.setUniformValue("mvp_matrix", mProjection * matrix);

    mArrayBuf.bind();
    int vertexLocation = mProgram.attributeLocation("aPosition");
    mProgram.enableAttributeArray(vertexLocation);
    mProgram.setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    mTexcoordBuf.bind();
    int texcoordLocation = mProgram.attributeLocation("aTexCoord");
    mProgram.enableAttributeArray(texcoordLocation);
    mProgram.setAttributeBuffer(texcoordLocation, GL_FLOAT, 0, 2, sizeof(QVector2D));

    mTexture->bind();
    mProgram.setUniformValue("uTexture", 0);

    mAngle = mAnimation->currentValue().toFloat();
    int angleLocation = mProgram.attributeLocation("aAngle");
    mProgram.setAttributeValue(angleLocation, mAngle);

    mIndexBuf.bind();
    glDrawElements(GL_TRIANGLES, mIndexBuf.size() / sizeof(GLushort), GL_UNSIGNED_SHORT, 0);

    if (mAnimation->state() == QAbstractAnimation::Running) {
        update();
    }
}

void FolderWidget::createPlaneMesh(float width, float height, int wResolution, int hResolution) {
    mNumOfVertices = (wResolution + 1) * (hResolution + 1);

    GLfloat *vertices = new GLfloat[mNumOfVertices * NUM_OF_VERTEX_XYZ];
    GLfloat *texcoords = new GLfloat[mNumOfVertices * NUM_OF_TEXTURE_ST];

    GLfloat halfWidth = width * 0.5f;
    GLfloat halfHeight = height * 0.5f;

    for (int y = 0, indexV = 0, indexT = 0; y <= hResolution; y++) {
        GLfloat normalizedY = 1.0f - (GLfloat)y / hResolution;
        GLfloat yOffset = normalizedY * height;

        for (int x = 0; x <= wResolution; x++) {
            GLfloat normalizedX = (GLfloat)x / wResolution;
            GLfloat xOffset = normalizedX * width;

            vertices[indexV++] = xOffset - halfWidth;
            vertices[indexV++] = yOffset - halfHeight;
            vertices[indexV++] = 0.0f;

            texcoords[indexT++] = normalizedX;
            texcoords[indexT++] = normalizedY;
        }
    }

    mArrayBuf.bind();
    mArrayBuf.allocate(vertices, mNumOfVertices * NUM_OF_VERTEX_XYZ * sizeof(GLfloat));

    mTexcoordBuf.bind();
    mTexcoordBuf.allocate(texcoords, mNumOfVertices * NUM_OF_TEXTURE_ST * sizeof(GLfloat));

    GLushort *indices = new GLushort[hResolution * wResolution * 6];
    for (int y = 0, idx = 0; y < hResolution; y++) {
        int curY = y * (wResolution + 1);
        int belowY = (y + 1) * (wResolution + 1);

        for (int x = 0; x < wResolution; x++) {
            int curV = curY + x;
            int belowV = belowY + x;

            indices[idx++] = (GLushort) curV;
            indices[idx++] = (GLushort) belowV;
            indices[idx++] = (GLushort) (curV + 1);

            indices[idx++] = (GLushort) belowV;
            indices[idx++] = (GLushort) (belowV + 1);
            indices[idx++] = (GLushort) (curV + 1);
        }
    }

    mIndexBuf.bind();
    mIndexBuf.allocate(indices, hResolution * wResolution * 6 * sizeof(GLushort));
}

void FolderWidget::mouseReleaseEvent(QMouseEvent *e) {
    mAnimation->start();
    update();
}
