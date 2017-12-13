#ifndef CUBEITEM_H
#define CUBEITEM_H
#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QQuickWindow>
#include <QThreadPool>
#include <QRunnable>
#include <QMutex>
#include <QNetworkAccessManager>
#include <functional>

class MyTask: public QObject,public QRunnable
{
    Q_OBJECT
public:
    MyTask(std::function<void()> f);
    virtual void run() override;
private:
    std::function<void()> m_f;
};

class CubeRender: public QQuickFramebufferObject::Renderer, public QOpenGLFunctions
{
public:
    CubeRender();
    void setTexture(const QString path);
    QString texture() const;
    void initialization();
    void render() override;
    QOpenGLFramebufferObject* createFramebufferObject(const QSize &size) override;
    void synchronize(QQuickFramebufferObject *item);
private:
    void makeMVP();
    QVector<QVector3D> m_vertexs;
    QVector<QVector2D> m_texCoords;
    int location_vertex;
    int location_texCoord;
    int location_matrix;
    int location_texture0;
    QOpenGLTexture *m_texture = nullptr;
    QString m_texturePath;
    bool m_textureDirty = true;
    QMatrix4x4 m_mvp;
    QOpenGLShaderProgram *m_program;
    float m_radius = 0;
    QSize m_size;
    QQuickWindow *window;
};

class CubeItem: public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(QString texturePath READ texturePath WRITE setTexturePath NOTIFY texturePathChanged)
public:
    CubeItem();
    QString texturePath() const;
    void setTexturePath(QString &path);
    Renderer *createRenderer() const;
    Q_INVOKABLE void setTexture(QString path);
    QImage textureImage();
    bool isTextureDirty();
    void setTextureDirty(bool what);
signals:
    void texturePathChanged(QString path);
    void textureImageReady();
private:
    QString m_texturePath;
    QImage m_textureImage;
    QThreadPool m_pool;
    QNetworkAccessManager m_manger;
    QMutex m_mutex;
    bool m_textureDirty = false;
};

#endif // CUBEITEM_H
