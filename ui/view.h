#ifndef VIEW_H
#define VIEW_H

#include "GL/glew.h"
#include <qgl.h>
#include <QTime>
#include <QTimer>
#include <memory>

#include "shapes/openglshape.h"
#include "camera/camera.h"
#include "gl/shader.h"
//#include "uniforms/uniformvariable.h"
#include <QTimer>
#include "shapes/quad.h"
#include "shapes/objmodel.h"
#include "gl/textures/Texture2D.h"
#include "gl/textures/shadowmapping.h"

class QGLShaderProgram;

class View : public QGLWidget {
    Q_OBJECT

public:
    View(QWidget *parent);
    ~View();

private:
    QTime m_time;
    QTimer m_timer;
    bool m_captureMouse;

    void initializeGL();
    void paintGL();
    void resizeGL(int w, int h);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

    std::unique_ptr<OpenGLShape> m_sphere;
    std::unique_ptr<Quad> m_quad;
    std::unique_ptr<ObjModel> m_character;

    std::unique_ptr<Shader> m_outline_shader;
    std::unique_ptr<Shader> m_toon_shader;
    std::unique_ptr<Shader> m_water_shader;
    std::unique_ptr<Shader> m_shadowmap_shader;

    std::shared_ptr<Texture2D> m_toon_diffuse;
    std::shared_ptr<ShadowMapping> m_shadow_map;

private slots:
    void tick();

private:
    int m_viewportWidth;
    int m_viewportHeight;
};

#endif // VIEW_H
