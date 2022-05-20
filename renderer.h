#ifndef RENDERER_H
#define RENDERER_H

#include "GL/glew.h"
#include "GL/glut.h"
#include <assimp\Importer.hpp>
#include <assimp\Exporter.hpp>
#include <assimp\scene.h>
#include <assimp\mesh.h>
#include <assimp/matrix4x4.h>
#include <assimp\config.h>
#include <assimp\material.h>
#include <assimp\cimport.h>
#include <assimp\postprocess.h>
#include <QVector>
#include <QObject>
#include <QCoreApplication>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <vector>
#include <QVector3D>
#include <QWidget>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLFunctions_4_5_Core>
#include <qopenglfunctions_4_5_compatibility.h>
#include <QOpenGLFunctions_ES2>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QTransform>
#include <QtMath>
#include <QVector2D>
#include <QVector4D>
#include <QOpenGLDebugLogger>
#include <QOpenGLTexture>
#include <QImage>
#include <QImageReader>


struct Vertex
{
    QVector3D vertex;
    QVector3D normal;
    QVector2D uv;
    QVector4D color;
};
struct triangle
{
    unsigned int vertex[3];

};

class Renderer : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Compatibility
{
    Q_OBJECT
public:
    explicit Renderer(QWidget *parent = nullptr);
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

    void PrepareModel();
    void setMatrix();
    void PrepareImage();
    GLenum glCheckError_(const char* file, int line);

    QMatrix4x4 FromMatrix(aiMatrix4x4 mat);
    QMatrix4x4 QMatrixMul(QMatrix4x4 m1,QMatrix4x4 m2);
    QVector3D FromVector(aiVector3D vec);
    QVector3D QMatrix4x4_Transform( QVector3D oPoint ,QMatrix4x4 matrix);
    QImage aiTexToQImage(aiTexture tex);

    int modelview=0;
    int projection=0;
    int view=0;
    int mvp=0;

    int vertexpos=0;
    int normalpos=0;
    int uvpos=0;
    int colorpos=0;
    QMatrix4x4 Modell;
    QMatrix4x4 Vieww;
    QMatrix4x4 Projectionn;
    QMatrix4x4 MVPp;

public slots:
    void ImportAssimp(void);


signals:


private:
    aiNode* root;
    const aiScene* scene;
    QOpenGLShaderProgram* Shader;
    void CreateShader();
    QTimer time;

    std::vector<GLfloat> m_geom_vertices;
    std::vector<GLfloat> m_geom_normals;
    std::vector<GLuint> m_geom_indices;
    std::vector<GLfloat> m_geom_uv;
    std::vector<GLfloat> m_geom_color;
    std::vector<std::string> textures;
    std::vector<QImage> texturesRaw;

    QOpenGLBuffer* vertexbuff;
    QOpenGLBuffer* normalbuff;
    QOpenGLBuffer* texturebuff;
    QOpenGLBuffer* colorbuff;
    QOpenGLBuffer* indexbuff;
    QOpenGLVertexArrayObject* m_vao;

};

#endif // RENDERER_H
