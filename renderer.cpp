#include "renderer.h"
#include <type_traits>
#define glCheckError() glCheckError_(__FILE__, __LINE__)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
int wi=0;
int he=0;

Renderer::Renderer(QWidget *parent)
    : QOpenGLWidget{parent}
{

    connect(&time,SIGNAL(timeout()),this,SLOT(update()));
    time.start(16);

}

void GLAPIENTRY MessageCallback( GLenum source,GLenum type,GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam )
{
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

void Renderer::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    ImportAssimp();
    PrepareImage();
    CreateShader();
    PrepareModel();
}

void Renderer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setMatrix();

    if(m_geom_indices.size()>0)
    {
 glBindTexture(GL_TEXTURE_2D, 1);
        m_vao->bind();
        glDrawElements(GL_TRIANGLES, (int)m_geom_indices.size(), GL_UNSIGNED_INT, 0);
        m_vao->release();
    }
    else
    {
        glBegin(GL_TRIANGLES);
              glColor3f(1.0, 0.0, 0.0);
              glVertex3f(-0.5, -0.5, 0);
              glColor3f(0.0, 1.0, 0.0);
              glVertex3f( 0.5, -0.5, 0);
              glColor3f(0.0, 0.0, 1.0);
              glVertex3f( 0.0,  0.5, 0);
          glEnd();
    }

}

void Renderer::resizeGL(int w, int h)
{
    wi=w;
    he=h;

}

void Renderer::PrepareModel()
{

    m_vao=new QOpenGLVertexArrayObject();
    m_vao->create();
    m_vao->bind();

    vertexbuff= new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    vertexbuff->create();
    vertexbuff->bind();
    vertexbuff->setUsagePattern(QOpenGLBuffer::StaticDraw);
    vertexbuff->allocate(&m_geom_vertices[0], (int)m_geom_vertices.size() * sizeof(GLfloat));

    Shader ->enableAttributeArray( vertexpos );
    Shader ->setAttributeBuffer(vertexpos,GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    Shader ->enableAttributeArray( normalpos );
    Shader ->setAttributeBuffer(normalpos,GL_FLOAT, 0, 3, 3 * sizeof(GLfloat));

    Shader ->enableAttributeArray( uvpos);
    Shader ->setAttributeBuffer(uvpos, GL_FLOAT, 0, 2, 2 * sizeof(GLfloat));


    indexbuff = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    indexbuff->create();
    indexbuff->bind();
    indexbuff->setUsagePattern(QOpenGLBuffer::StaticDraw);   
    indexbuff->allocate(&m_geom_indices[0], (int)m_geom_indices.size() * sizeof(GLuint));
    m_vao->release();
    qDebug() << "Model Prepared";
}


void Renderer::setMatrix()
{

        qreal aspect = qreal(wi) / qreal(he ? he : 1);
        const qreal zNear = 3.0, zFar = 7.0, fov = 45.0;

        QMatrix4x4 matrix;
        matrix.translate(0.0, 0.0, -5.0);

        Projectionn.setToIdentity();
        Projectionn.perspective(fov, aspect, zNear, zFar);
        Shader->setUniformValue(projection,Projectionn*matrix);

}

void Renderer::PrepareImage()
{
    if(textures.empty() == false)
    {
        GLuint texture[sizeof(textures)];
        glGenTextures(textures.size(), texture);
        for(int t=0;t< textures.size();t++)
        {
            int width, height, bpp;
            stbi_set_flip_vertically_on_load(true);
            uint8_t* rgb_image = stbi_load(textures[t].c_str(), &width, &height, &bpp, STBI_rgb_alpha);
            if (rgb_image == nullptr)
             {
                 qDebug() << "cant data";

             }
            else
            {
                glBindTexture(GL_TEXTURE_2D, texture[t]);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgb_image);

                stbi_image_free(rgb_image);
                qDebug() << "textures applied";

            }
        }
    }
}


void Renderer::ImportAssimp()
{

    Assimp::Importer importer;
    QString path=QCoreApplication::applicationDirPath()+"/ball.fbx";
    scene = importer.ReadFile(path.toStdString(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
    root = scene->mRootNode;
    if (scene)
    {
        for (size_t i = 0; i < scene->mNumMeshes; i++)
          {
            // Populate vertices
            for (size_t j = 0; j < scene->mMeshes[i]->mNumVertices; j++)
            {
              m_geom_vertices.push_back(scene->mMeshes[i]->mVertices[j].x);
              m_geom_vertices.push_back(scene->mMeshes[i]->mVertices[j].y);
              m_geom_vertices.push_back(scene->mMeshes[i]->mVertices[j].z);

              m_geom_normals.push_back(scene->mMeshes[i]->mNormals[j].x);
              m_geom_normals.push_back(scene->mMeshes[i]->mNormals[j].y);
              m_geom_normals.push_back(scene->mMeshes[i]->mNormals[j].z);

              if (scene->mMeshes[i]->mTextureCoords[0])
              {
                 m_geom_uv.push_back( scene->mMeshes[i]->mTextureCoords[0][j].x);
                 m_geom_uv.push_back( scene->mMeshes[i]->mTextureCoords[0][j].y);
              }
              if (scene->mMeshes[i]->HasVertexColors(0))
              {
                 m_geom_color.push_back( scene->mMeshes[i]->mColors[0][j].r);
                 m_geom_color.push_back( scene->mMeshes[i]->mColors[0][j].g);
                 m_geom_color.push_back( scene->mMeshes[i]->mColors[0][j].b);
                 m_geom_color.push_back( scene->mMeshes[i]->mColors[0][j].a);
              }

            }

            // Populate indices
            for (size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
            {
              m_geom_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[0]);
              m_geom_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[1]);
              m_geom_indices.push_back(scene->mMeshes[i]->mFaces[j].mIndices[2]);
            }
          }

        if (scene->HasTextures())
                    {

                        for (unsigned int m = 0; m < scene->mNumMaterials; m++)
                        {
                            auto material = scene->mMaterials[m];

                            int texIndex = 0;
                            aiReturn texFound = AI_SUCCESS;

                            aiString path;	// filename

                            while (texFound == AI_SUCCESS)
                            {
                                texFound = material->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
                                textures.push_back(path.C_Str());
                                texIndex++;
                            }
                        }

                    }
    }
}

void Renderer::CreateShader()
{
    Shader = new QOpenGLShaderProgram();
    QString path1=QCoreApplication::applicationDirPath()+"/vertex.glsl";
    QString path2=QCoreApplication::applicationDirPath()+"/fragment.glsl";
    Shader->addShaderFromSourceFile(QOpenGLShader::Vertex, path1);
    Shader->addShaderFromSourceFile(QOpenGLShader::Fragment, path2);


    Shader->link();
    if(Shader->isLinked()==true)
    {
        qDebug() << "Shader is linked";
    }
    Shader -> bind();

    //modelview = Shader->uniformLocation("modelview");
    projection = Shader->uniformLocation("projection");
    //view = Shader->uniformLocation("view");
    //mvp =  Shader->uniformLocation("mvp");

    vertexpos = Shader->attributeLocation("position");
    normalpos = Shader->attributeLocation("normal");
    uvpos = Shader->attributeLocation("textureCoordinates");
    colorpos =  Shader->attributeLocation("colorr");

}

//
GLenum Renderer::glCheckError_(const char* file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
       qDebug()<< "Error " << error.c_str() << " on file " << file << " on line "<< line ;
    }
    return errorCode;
}

QMatrix4x4 Renderer::FromMatrix(aiMatrix4x4 mat)
{
    QMatrix4x4 m {mat.a1,mat.a2,mat.a3,mat.a4,mat.b1,mat.b2,mat.b3,mat.b4,mat.c1,mat.c2,mat.c3,mat.c4,mat.d1,mat.d2,mat.d3,mat.d4} ;
    return m;
}
QMatrix4x4 Renderer::QMatrixMul(QMatrix4x4 m1,QMatrix4x4 m2)
{
    QMatrix4x4 m ;
    m.column(0).setX(m1.column(0).x()*m2.column(0).x());
    m.column(0).setY(m1.column(0).y()*m2.column(0).y());
    m.column(0).setZ(m1.column(0).z()*m2.column(0).z());
    m.column(0).setW(m1.column(0).w()*m2.column(0).w());

    m.column(1).setX(m1.column(1).x()*m2.column(1).x());
    m.column(1).setY(m1.column(1).y()*m2.column(1).y());
    m.column(1).setZ(m1.column(1).z()*m2.column(1).z());
    m.column(1).setW(m1.column(1).w()*m2.column(1).w());

    m.column(2).setX(m1.column(2).x()*m2.column(2).x());
    m.column(2).setY(m1.column(2).y()*m2.column(2).y());
    m.column(2).setZ(m1.column(2).z()*m2.column(2).z());
    m.column(2).setW(m1.column(2).w()*m2.column(2).w());

    m.column(3).setX(m1.column(3).x()*m2.column(3).x());
    m.column(3).setY(m1.column(3).y()*m2.column(3).y());
    m.column(3).setZ(m1.column(3).z()*m2.column(3).z());
    m.column(3).setW(m1.column(3).w()*m2.column(3).w());

    return m;
}
QVector3D Renderer::FromVector(aiVector3D vec)
{
    QVector3D v{vec.x,vec.y,vec.z};
    return v;
}
QVector3D Renderer::QMatrix4x4_Transform( QVector3D oPoint ,QMatrix4x4 matrix)
{
    float fX = oPoint.x();
    float fY = oPoint.y();
    float fZ = oPoint.z();

    oPoint.setX( fX * matrix.column(0)[0] + fY * matrix.column(0)[1] + fZ * matrix.column(0)[2] + matrix.column(0)[3]);
    oPoint.setY( fX * matrix.column(1)[0] + fY * matrix.column(1)[1] + fZ * matrix.column(1)[2] + matrix.column(1)[3]);
    oPoint.setZ( fX * matrix.column(2)[0] + fY * matrix.column(2)[1] + fZ * matrix.column(2)[2] + matrix.column(2)[3]);
    return oPoint;
}

QImage Renderer::aiTexToQImage(aiTexture tex)
{
    uint textureSize = tex.mWidth * tex.mHeight;
    char *textureContent = new char[textureSize * 4];

    for (uint i = 0; i < textureSize; i++)
    {
      uint idx = i * 4;
      auto texel = tex.pcData[i];
      textureContent[idx] = texel.r;
      textureContent[idx + 1] = texel.g;
      textureContent[idx + 2] = texel.b;
      textureContent[idx + 3] = texel.a;

    }
 QImage newimage((const unsigned char*)textureContent,tex.mWidth, tex.mHeight, QImage::Format_RGBA8888);

 return newimage;
}
