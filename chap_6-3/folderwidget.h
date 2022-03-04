#ifndef FOLDERWIDGET_H
#define FOLDERWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVariantAnimation>
#include <QtMath>

class FolderWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    FolderWidget(QWidget *parent = nullptr);
    ~FolderWidget();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void mouseReleaseEvent(QMouseEvent *e);

private:
    void initShader();
    void createPlaneMesh(float width, float height, int wResolution, int hResolution);

    QOpenGLBuffer mArrayBuf;
    QOpenGLBuffer mIndexBuf;
    QMatrix4x4 mProjection;
    QOpenGLShaderProgram mProgram;
    QOpenGLTexture *mTexture;
    QOpenGLBuffer mTexcoordBuf;

    int mWidth, mHeight;
    float mAngle;
    int mNumOfVertices;
    bool mChange;
    QVariantAnimation *mAnimation;
    const int DURATION = 1000;
    const float START_VALUE = 0.0f;
    const float END_VALUE = M_PI / 2.0f;
    const int NUM_OF_VERTEX_XYZ = 3;
    const int NUM_OF_TEXTURE_ST = 2;
};
#endif // FOLDERWIDGET_H
