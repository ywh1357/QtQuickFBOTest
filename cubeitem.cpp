#include "cubeitem.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QRunnable>
#include <QEventLoop>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMutexLocker>
#include <memory>

CubeRender::CubeRender(){
    initialization();
}

void CubeRender::makeMVP()
{
    qDebug() << "makeMVP with " << m_size;
    QMatrix4x4 model;
    model.rotate(50,0.5,1.0,0);
    QMatrix4x4 view;
    view.translate(0.0f, 0.0f, -3.0f);
    QMatrix4x4 projection;
    projection.perspective(45, m_size.width() / m_size.height(), 0.1f, 100.0f);
    m_mvp = projection * view * model;
}

void CubeRender::setTexture(const QString path){
    m_textureDirty = true;
    m_texturePath = path;
}

QString CubeRender::texture() const
{
    return m_texturePath;
}

void CubeRender::initialization(){
    qDebug() << "initialization";

    initializeOpenGLFunctions();

    m_program  = new QOpenGLShaderProgram();
    m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Vertex,":/cubevs.vsh");
    m_program->addCacheableShaderFromSourceFile(QOpenGLShader::Fragment,":/cubefs.fsh");
    m_program->link();

    location_vertex = m_program->attributeLocation("Vertex");
    location_texCoord = m_program->attributeLocation("aTexCoord");
    location_matrix = m_program->uniformLocation("mvp");
    location_texture0 = m_program->uniformLocation("Texture0");

//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    float vertices[] = {
        // positions
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,

         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f,  0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f,  0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f, -0.5f,
    };
    float texCoords[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };

    auto vertexLen = sizeof(vertices)/sizeof(float);

    for(int i = 0; i < vertexLen; i += 3){
        m_vertexs.push_back(QVector3D(vertices[i],vertices[i+1],vertices[i+2]));
    }

    auto texCoordsLen = sizeof(texCoords)/sizeof(float);

    for(int i = 0; i < texCoordsLen; i += 2){
        m_texCoords.push_back(QVector2D(texCoords[i],texCoords[i+1]));
    }

    qDebug() << m_vertexs.size() << m_texCoords.size();

    m_program->enableAttributeArray(location_vertex);
    m_program->enableAttributeArray(location_texCoord);
    m_program->setAttributeArray(location_vertex,m_vertexs.constData());
    m_program->setAttributeArray(location_texCoord,m_texCoords.constData());
    m_program->disableAttributeArray(location_vertex);
    m_program->disableAttributeArray(location_vertex);

//    makeMVP();
}

void CubeRender::render(){

    if(!m_texture){
        return;
    }

    QMatrix4x4 rotation;
    rotation.rotate(m_radius,0,1,0);
    QMatrix4x4 mvp;
    mvp = m_mvp * rotation;
    m_radius += 0.1f;

//    if (m_textureDirty) {
//        if (m_texture)
//            delete m_texture;
              //...
//        qDebug() << m_texturePath;
//        m_textureDirty = false;
//    }

//    glDepthMask(true);

//    glViewport(0,0,m_size.width(),m_size.height());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

//    glEnable(GL_CULL_FACE);
//    glFrontFace(GL_CW);
//    glCullFace(GL_BACK);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    m_program->bind();
    m_program->setUniformValue(location_matrix, mvp);
    m_program->setUniformValue(location_texture0,1);
    m_texture->bind(1);
    m_program->enableAttributeArray(location_vertex);
    m_program->enableAttributeArray(location_texCoord);
    m_program->setAttributeArray(location_vertex,m_vertexs.constData());
    m_program->setAttributeArray(location_texCoord,m_texCoords.constData());
    glDrawArrays(GL_TRIANGLES,0,m_vertexs.size());
    m_program->disableAttributeArray(location_vertex);
    m_program->disableAttributeArray(location_vertex);
    m_texture->release();
    m_program->release();

//    glFlush();

    window->resetOpenGLState();
    update();
}

QOpenGLFramebufferObject *CubeRender::createFramebufferObject(const QSize &size)
{
    m_size = size;
    makeMVP();
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(16);
    return new QOpenGLFramebufferObject(size, format);
}

void CubeRender::synchronize(QQuickFramebufferObject *item)
{
    CubeItem *cube = static_cast<CubeItem*>(item);
    if(cube->isTextureDirty()){
        delete m_texture;
        m_texture = nullptr;
        m_texture = new QOpenGLTexture(cube->textureImage().mirrored());
        m_texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
        m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
        cube->setTextureDirty(false);
    }
    window = cube->window();
}

CubeItem::CubeItem()
{
    setMirrorVertically(true);
    connect(this,&CubeItem::textureImageReady,this,&QQuickItem::update);
}

QString CubeItem::texturePath() const
{
    return m_texturePath;
}

void CubeItem::setTexturePath(QString &path)
{
    if(m_texturePath != path){
        m_texturePath = path;
        emit texturePathChanged(m_texturePath);
        setTexture(path);
    }
}

QQuickFramebufferObject::Renderer *CubeItem::createRenderer() const
{
    return new CubeRender;
}

void CubeItem::setTexture(QString path)
{
    if(!QUrl::fromUserInput(path).isValid()){
        qDebug() << "Texture url invalid.";
        return;
    }
    m_pool.start(new MyTask([this,path]{
        auto url = QUrl::fromUserInput(path);
        qDebug() << "Texture Url: " << url;
        if(url.isLocalFile()){
            qDebug() << "load from loacl file: " << url.toLocalFile();
            m_mutex.lock();
            m_textureImage.load(url.toLocalFile());
            m_textureDirty = true;
            m_mutex.unlock();
            emit textureImageReady();
        }else{
			std::shared_ptr<QNetworkAccessManager> manager = std::make_shared<QNetworkAccessManager>();
			auto reply = manager->get(QNetworkRequest(url));
			qDebug() << "Network request for " << url;
			//qDebug() << reply->parent() << manager.get();
            //QNetworkReply *reply = m_manger.get(QNetworkRequest(url));
            QEventLoop loop;
            QObject::connect(reply,&QNetworkReply::finished,[this,reply,&loop]{
                if(reply->error()){
                    qDebug() << reply->errorString();
                }else{
                    m_mutex.lock();
                    m_textureImage.loadFromData(reply->readAll());
                    m_textureDirty = true;
                    m_mutex.unlock();
                    qDebug() << "texture ready";
                    emit textureImageReady();
                }
                loop.exit();
				qDebug() << "loop exit";
            });
            loop.exec();
        }
    }));
}

QImage CubeItem::textureImage()
{
    QMutexLocker locker(&m_mutex);
    QImage temp(m_textureImage);
    return temp;
}

bool CubeItem::isTextureDirty()
{
    QMutexLocker locker(&m_mutex);
    return m_textureDirty;
}

void CubeItem::setTextureDirty(bool what)
{
    QMutexLocker locker(&m_mutex);
    m_textureDirty = what;
}

MyTask::MyTask(std::function<void ()> f):m_f(f),QObject(nullptr)
{
    setAutoDelete(true);
}

void MyTask::run()
{
    m_f();
}
